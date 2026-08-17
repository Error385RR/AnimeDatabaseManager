#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "pkce.hpp"
#include "http_client.hpp"
#include "parameterbuilder.hpp"
#include "oauth_config.hpp"
#include "json.hpp"

using json = nlohmann::json;



class OAuth2Client{
public:
    OAuth2Client(const OAuthConfig& oauthConfig) 
        : config(oauthConfig)      
    {
        if(config.clientID.empty())
        {
            config.clientID = getClientID(config.clientIDfilepath);
        }
    }

    static std::string getClientID(const std::string& filepath){
        std::string clientidfromfile;
        std::fstream clientidfile;
        clientidfile.open(filepath, std::ios::in);

        if(!clientidfile.is_open())
        {
            throw std::runtime_error("Could not open client ID file");
        }

        std::getline(clientidfile, clientidfromfile);

        return clientidfromfile;
    }

    std::string pkceVerifierGenerator(){
        return pkce.generateVerifier();
    }

    std::string pkceCodeChallenge(const std::string& verifier){return pkce.createCodeChallenge(verifier);}

    std::string generateState(){return pkce.generateVerifier();}

    std::string generateAuthorizationURL(){
        genericParameterBuilder builder(config.authorizationEndpoint);
        
        builder.addParam("client_id", config.clientID);
        builder.addParam("response_type", "code");
        builder.addParam("redirect_uri", config.redirectURI);
        if(!config.scope.empty()){builder.addParam("scope", config.scope);}
        if(config.state){builder.addParam("state", generateState());}
        
        if(config.usePKCE)
        {
            verifier = pkceVerifierGenerator();
            if (config.codeChallengeMethod == PKCEMethod::SHA256)
            {
                codeChallenge = pkceCodeChallenge(verifier);
                codeChallengeMethodString = "S256";
            }else{
                codeChallenge = verifier;
                codeChallengeMethodString = "plain";
            }
            
            builder.addParam("code_challenge", codeChallenge);
            builder.addParam("code_challenge_method", codeChallengeMethodString);
        }

       return builder.buildQueryString();
    }

    std::string generateTokenBody(){
        genericParameterBuilder builder(config.tokenEndpoint);

        builder.addParam("client_id", config.clientID);
        builder.addParam("grant_type", "authorization_code");
        builder.addParam("redirect_uri", config.redirectURI);
        builder.addParam("code", codeForToken);
        if(config.usePKCE){builder.addParam("code_verifier", verifier);}
        if(!config.clientSecret.empty()){builder.addParam("client_secret", config.clientSecret);}
        return builder.buildFormBody();
    }

    json exchangeCodeForToken(){
        std::vector<httphandler::HttpHeader> headers{
            {"Content-Type", "application/x-www-form-urlencoded"}
        };
        std::string tokenBody = generateTokenBody();
        httphandler::HttpResponse tokenGeneratedstruct = httpHandler.exchangeToken(tokenBody,config.tokenEndpoint, headers);
        
        std::string tokenGenerated = tokenGeneratedstruct.body;
        
        json tokenJson = json::parse(tokenGenerated);

        return tokenJson;
    }

    json authenticate(){
        std::string authURL = generateAuthorizationURL();
        //temporary
        std::cout <<"Click On The URL and Grant Access In The Browser: "<< authURL <<std::endl;
        codeForToken = httpHandler.waitForAuthorizationCode();
    
        json tokenJson = exchangeCodeForToken();

        return tokenJson;
    }

    void printConfigStruct(){
        std::cout << config.baseURL<<std::endl;
        std::cout << config.tokenEndpoint <<std::endl;
        std::cout << config.clientID <<std::endl;
        std::cout << config.clientSecret <<std::endl;
        std::cout << config.redirectURI <<std::endl;
        std::cout << config.clientID <<std::endl;
        std::cout << config.usePKCE <<std::endl;
        std::cout << config.state <<std::endl;
        std::cout << static_cast<int>(config.codeChallengeMethod) <<std::endl;
    }

private:
    httphandler httpHandler;
    PKCE pkce;
    
    std::string verifier;
    std::string codeChallenge;
    std::string codeChallengeMethodString;
    std::string codeForToken;
   
    OAuthConfig config;
};