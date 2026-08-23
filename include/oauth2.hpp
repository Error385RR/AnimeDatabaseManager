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
    struct OAuthFlow{
        std::string state;    
        std::string verifier;
        std::string codeChallenge;
        std::string codeChallengeMethodString;
        std::string codeForToken;
    };
    OAuth2Client(const OAuthConfig& oauthConfig);

    static std::string getClientID(const std::string& filepath);
    // std::string pkceVerifierGenerator();

    // std::string pkceCodeChallenge(const std::string& verifier);
    static std::string generateState();

    std::string generateAuthorizationURL();

    std::string generateTokenBody();
    json exchangeCodeForToken(const std::vector<httphandler::HttpHeader>& headers = {});

    json authenticate();

    void printConfigStruct();

private:
    httphandler httpHandler;
    // PKCE pkce;
    OAuthFlow flow;
   
    OAuthConfig config;
};