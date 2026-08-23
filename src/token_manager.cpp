
#include <iostream>
#include "token_manager.hpp"
#include "oauth2.hpp"

#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>


#include "oauth_config.hpp"

using json = nlohmann::json;

TokenManager::TokenManager(
    const std::filesystem::path& filepath)
    : tokenFileJsonPath(filepath) 
{}

bool TokenManager::isValidToken(const OAuthToken& token){
    auto now = std::chrono::system_clock::now();
    return now + std::chrono::minutes(5) < token.expiresin;
}

bool TokenManager::isUsableToken(const OAuthToken& token){
    return
    !token.accessToken.empty() &&
    !token.refreshToken.empty() &&
    !token.tokenType.empty();
    
}
bool TokenManager::canUseAccessToken(const OAuthToken& token)
{
    return isUsableToken(token) &&
           isValidToken(token);
}

TokenManager::OAuthToken TokenManager::loadTokenfiledata(){
    std::ifstream tfile;
    std::string jsonToken;
    TokenManager::OAuthToken token;
    
    if(!std::filesystem::exists(tokenFileJsonPath)){
        std::cout << "Current directory: "<< std::filesystem::current_path() << '\n';
        std::cout << "Looking for: "<< std::filesystem::absolute(tokenFileJsonPath) << '\n';
        std::cout << std::boolalpha<< "Exists: "<< std::filesystem::exists(tokenFileJsonPath) << '\n';
        throw std::runtime_error("File Does Not Exist");
    }else{
        tfile.open(tokenFileJsonPath,std::ios_base::in);
    }
    
    json jsonTokenData;
    tfile >> jsonTokenData;

    if (!jsonTokenData.contains("access_token") ||
        !jsonTokenData["access_token"].is_string() ||
        jsonTokenData["access_token"].get<std::string>().empty())
    {
        throw std::runtime_error(
            "Invalid token file: access_token"
        );
    }

    if (!jsonTokenData.contains("refresh_token") ||
        !jsonTokenData["refresh_token"].is_string() ||
        jsonTokenData["refresh_token"].get<std::string>().empty())
    {
        throw std::runtime_error(
            "Invalid token file: refresh_token"
        );
    }

    if (!jsonTokenData.contains("token_type") ||
        !jsonTokenData["token_type"].is_string() ||
        jsonTokenData["token_type"].get<std::string>().empty())
    {
        throw std::runtime_error(
            "Invalid token file: token_type"
        );
    }

    if (!jsonTokenData.contains("expires_at") ||
        !jsonTokenData["expires_at"].is_number_integer())
    {
        throw std::runtime_error(
            "Invalid token file: expires_at"
        );
    }
    token.accessToken =
        jsonTokenData["access_token"].get<std::string>();

    token.refreshToken =
        jsonTokenData["refresh_token"].get<std::string>();

    token.tokenType =
        jsonTokenData["token_type"].get<std::string>();

    token.expiresin =
        std::chrono::system_clock::from_time_t(
            jsonTokenData["expires_at"].get<std::time_t>()
        );
    tfile.close();

    token.isValid = TokenManager::isValidToken(token);
    if (token.isValid == false)
    {
        return token;
    }
    
    return token;

}

void TokenManager::saveToken(const TokenManager::OAuthToken& token){
    std::ofstream tfile;
    json jsonToken;
    tfile.open(tokenFileJsonPath, std::ios::trunc);
    if (!tfile)
    {
        throw std::runtime_error(
            "Failed to open token file for writing"
        );
    }
    jsonToken["access_token"] = token.accessToken;
    jsonToken["refresh_token"] = token.refreshToken;
    jsonToken["token_type"] = token.tokenType;
    jsonToken["expires_at"] = std::chrono::system_clock::to_time_t(token.expiresin);
    
    tfile << jsonToken.dump(3);
    tfile.close();
}

TokenManager::OAuthToken TokenManager::refreshToken(const OAuthConfig& config, const std::string& refreshToken){
    TokenManager::OAuthToken newtoken;
    genericParameterBuilder builder(config.tokenEndpoint);
    httphandler::HttpHeader header = {"Content-Type", "application/x-www-form-urlencoded"};
    std::vector<httphandler::HttpHeader> headers;
    headers.push_back(header);

    builder.addParam("grant_type","refresh_token" );
    builder.addParam("refresh_token", refreshToken);
    builder.addParam("client_id", OAuth2Client::getClientID(config.clientIDfilepath));
    auto body = builder.buildFormBody();

    auto response = httpHandler.post(config.tokenEndpoint, headers, body);

    json jsonTokenData =  json::parse(response.body);
    newtoken.accessToken = jsonTokenData["access_token"];
    if(jsonTokenData.contains("refresh_token"))
    {
        newtoken.refreshToken = jsonTokenData["refresh_token"];
    }
    else
    {
        newtoken.refreshToken = refreshToken;
    }
        newtoken.tokenType = jsonTokenData["token_type"];

    newtoken.expiresin =
        std::chrono::system_clock::now() +
        std::chrono::seconds(
            jsonTokenData["expires_in"].get<int>()
        );
    newtoken.isValid = true;

    saveToken(newtoken);
    
    return newtoken;
}

TokenManager::OAuthToken TokenManager::getTokenData(OAuth2Client& maloauth, const OAuthConfig& config){
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
            saveToken(token);
        }
        
    }
    
    if(!token.isValid){
        auto newToken = refreshToken(config, token.refreshToken);
        return newToken;
    }
            
    return token;
}
