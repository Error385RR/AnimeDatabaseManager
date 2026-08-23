#pragma once


#include "oauth2.hpp"
#include "token_manager.hpp"
#include "http_client.hpp"
#include "json.hpp"
#include "oauth_config.hpp"
#include "config.hpp"
#include "IAnimeProvider.hpp"
using json = nlohmann::json;

class MalProvider : public IAnimeProvider
{
private:
    httphandler malhttphandler;
    // std::filesystem::path jsonFilepath;
    OAuthConfig config;
    OAuth2Client maloauth;
    TokenManager tokenManager;

    static OAuthConfig createOAuthConfig(
        const Config::configdata& cfg
    );

public:
    MalProvider(const Config::configdata& cfg);
    TokenManager::OAuthToken getTokenData();


    anime::Anime getAnimeById(int id) override;

    json getUserAnimeList();

    json getMangabyId(int id);

    json getUserAnimeList(const std::string& username);

    SearchPage searchAnimeByName(const std::string& searchquery) override;
    SearchPage getSearchPage(const std::string& nextPageURL) override;
    SearchPage parseSearchResponse(const std::string& body);
};