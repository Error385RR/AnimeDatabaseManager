#pragma once


#include "oauth2.hpp"
#include "token_manager.hpp"
#include "http_client.hpp"
#include "json.hpp"
#include "oauth_config.hpp"

using json = nlohmann::json;

class MalClient
{
private:
    httphandler malhttphandler;
    std::filesystem::path jsonFilepath = "D:/Dev/Projects/cli_dbexp/config/Maltoken.json";
    OAuth2Client maloauth;
    TokenManager tokenManager;

    static OAuthConfig createOAuthConfig();
    OAuthConfig config = createOAuthConfig();


public:
    MalClient();
    TokenManager::OAuthToken getTokenData();


    json getAnimebyId(int id);

    json getUserAnimeList();

    json getMangabyId(int id);

    json getUserAnimeList(const std::string& username);

    json searchAnime(const std::string& searchquery);

};