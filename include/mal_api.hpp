#pragma once


#include "oauth2.hpp"
#include "token_manager.hpp"
#include "http_client.hpp"
#include "json.hpp"
#include "oauth_config.hpp"
#include "config.hpp"

using json = nlohmann::json;

class MalClient
{
private:
   httphandler malhttphandler;
    std::filesystem::path jsonFilepath;
    OAuthConfig config;
    OAuth2Client maloauth;
    TokenManager tokenManager;

    static OAuthConfig createOAuthConfig(
        const Config::configdata& cfg
    );

public:
    MalClient(const Config::configdata& cfg);
    TokenManager::OAuthToken getTokenData();


    json getAnimebyId(int id);

    json getUserAnimeList();

    json getMangabyId(int id);

    json getUserAnimeList(const std::string& username);

    json searchAnime(const std::string& searchquery);

};