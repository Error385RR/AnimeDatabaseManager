#pragma once

#include <string>
#include <filesystem>
#include <chrono>
#include "json.hpp"
#include "oauth_config.hpp"

using json = nlohmann::json;
class OAuth2Client;

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
    explicit TokenManager(const std::filesystem::path& filepath);

    
    bool isValidToken(const OAuthToken& token);  
    OAuthToken loadTokenfiledata(); 
    void saveToken(const OAuthToken& token, std::filesystem::path& jsonfilepath);
    OAuthToken refreshToken(const OAuthConfig& config, const std::string& refreshToken);
    OAuthToken getTokenData(OAuth2Client& maloauth, const OAuthConfig& config);


};