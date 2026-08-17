#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "json.hpp"
#include "oauth2.hpp"

using json = nlohmann::json;

class TokenManager{
private:
    std::filesystem::path tokenFileJsonPath;

public:  
    struct OAuthToken{
        std::string accessToken;
        std::string refreshToken;
        std::string tokenType;
        std::chrono::system_clock::time_point expiresin;
        bool isValid;
    };

    TokenManager(const std::filesystem::path& filepath) :tokenFileJsonPath(filepath)
    {}
    
    bool isValidToken(const OAuthToken& token){
        auto now = std::chrono::system_clock::now();
        return now + std::chrono::minutes(5) < token.expiresin;
    }
    
    OAuthToken loadTokenfiledata(){
        std::ifstream tfile;
        std::string jsonToken;
        OAuthToken token;
        
        if(!std::filesystem::exists(tokenFileJsonPath)){
            std::cout << "Current directory: "<< std::filesystem::current_path() << '\n';
            std::cout << "Looking for: "<< std::filesystem::absolute(tokenFileJsonPath) << '\n';
            std::cout << std::boolalpha<< "Exists: "<< std::filesystem::exists(tokenFileJsonPath) << '\n';
            throw std::runtime_error("File Does Not Exist");
        }else{
            tfile.open(tokenFileJsonPath,std::ios_base::in);
        }
        
        auto jsonTokenData = json::parse(tfile);

        token.accessToken =jsonTokenData["access_token"];
        token.refreshToken = jsonTokenData["refresh_token"];
        token.tokenType = jsonTokenData["token_type"];
        token.expiresin =    std::chrono::system_clock::from_time_t(jsonTokenData["expires_at"].get<std::time_t>());
        tfile.close();

        token.isValid = isValidToken(token);
        if (token.isValid == false)
        {
            return token;
        }
        
        return token;
    }
    
    void saveToken(const OAuthToken& token, std::filesystem::path& jsonfilepath){
        std::ofstream tfile;
        json jsonToken;
        tfile.open(jsonfilepath, std::ios::trunc);
        jsonToken["access_token"] = token.accessToken;
        jsonToken["refresh_token"] = token.refreshToken;
        jsonToken["token_type"] = token.tokenType;
        jsonToken["expires_at"] = std::chrono::system_clock::to_time_t(token.expiresin);
      
        tfile << jsonToken.dump(3);
        tfile.close();
    }

    OAuthToken refreshToken(const OAuth2Client::OAuthConfig& config, const std::string& refreshToken){
        TokenManager::OAuthToken newtoken;
        genericParameterBuilder builder(config.tokenEndpoint);
        httphandler::HttpHeader header = {"Content-Type", "application/x-www-form-urlencoded"};
        std::vector<httphandler::HttpHeader> headers;
        headers.push_back(header);

        builder.addParam("grant_type","refresh_token" );
        builder.addParam("refresh_token", refreshToken);
        builder.addParam("client_id", OAuth2Client::getClientID(config.clientIDfilepath));
        auto body = builder.buildFormBody();

        auto response = httphandler::post(config.tokenEndpoint, headers, body);

        json jsonTokenData = json::parse(response.body);
        newtoken.accessToken = jsonTokenData["access_token"];
        if (!newtoken.refreshToken.empty())
        {
            newtoken.refreshToken = jsonTokenData["refresh_token"];
        }
         newtoken.tokenType = jsonTokenData["token_type"];

        newtoken.expiresin =
            std::chrono::system_clock::now() +
            std::chrono::seconds(
                jsonTokenData["expires_in"].get<int>()
            );
        newtoken.isValid = true;

        saveToken(newtoken, tokenFileJsonPath);
        
        return newtoken;
    }

    OAuthToken getTokenData(OAuth2Client& maloauth, const OAuth2Client::OAuthConfig& config){
        TokenManager::OAuthToken token;
        json jsonTokenData;
        try
        {
            token = loadTokenfiledata();   
        }
        catch(const std::exception& e)
        {
            if(std::string(e.what()) == "File Does Not Exist"){
                jsonTokenData = maloauth.authenticate();
                token.accessToken = jsonTokenData["access_token"];
                token.refreshToken = jsonTokenData["refresh_token"];
                token.tokenType = jsonTokenData["token_type"];
                token.expiresin =
                    std::chrono::system_clock::now() +
                    std::chrono::seconds(
                        jsonTokenData["expires_in"].get<int>()
                    );
                token.isValid = true;
                saveToken(token, tokenFileJsonPath);
            }
            
        }
        
        if(!token.isValid){
            auto newToken = refreshToken(config, token.refreshToken);
            return newToken;
        }
                
        return token;
}



};