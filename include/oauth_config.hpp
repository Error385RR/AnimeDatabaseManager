#pragma once
#include <iostream>

enum class PKCEMethod
{
    Plain,
    SHA256
};
struct OAuthConfig
{
    std::string baseURL;
    std::string authorizationEndpoint;
    std::string tokenEndpoint;
    std::string clientID;
    std::string clientIDfilepath;
    std::string clientSecret;
    std::string redirectURI;
    std::string scope;
    
    bool state = false;
    bool usePKCE = true;
    PKCEMethod codeChallengeMethod = PKCEMethod::Plain;
};
