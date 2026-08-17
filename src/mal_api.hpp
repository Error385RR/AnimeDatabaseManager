#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
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
    std::filesystem::path jsonFilepath = "./config/Maltoken.json";
    OAuth2Client maloauth;
    TokenManager tokenManager;

    static OAuthConfig createOAuthConfig()
    {
        OAuthConfig config;

        config.authorizationEndpoint =
            "https://myanimelist.net/v1/oauth2/authorize";

        config.tokenEndpoint =
            "https://myanimelist.net/v1/oauth2/token";

        config.usePKCE = true;

        config.codeChallengeMethod =
            PKCEMethod::Plain;

        config.redirectURI =
            "http://localhost:8080/callback";

        config.clientIDfilepath = "D:/Dev/Projects/cli_dbexp/config/Malclient.txt";
        
        return config;
    }
    OAuthConfig config = createOAuthConfig();


public:
    MalClient()
        : maloauth(createOAuthConfig()),
          tokenManager("./config/Maltoken.json")
    {
    }
    TokenManager::OAuthToken getTokenData(){
        TokenManager::OAuthToken token;
        json jsonTokenData;
        try
        {
            token = tokenManager.loadTokenfiledata();   
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
                tokenManager.saveToken(token, jsonFilepath);
            }
            
        }
        
        if(!token.isValid){
           auto newToken = tokenManager.refreshToken(config, token.refreshToken);
            return newToken;
        }
              
        return token;
    }


    json getAnimebyId(int id)
    {
            std::string baseURL ="https://api.myanimelist.net/v2/anime/" + std::to_string(id);
            genericParameterBuilder url(baseURL);
            url.addParam("fields", "id,title,main_picture,alternative_titles,start_date,end_date,synopsis,mean,rank,popularity,num_list_users,num_scoring_users,nsfw,created_at,updated_at,media_type,status,genres,my_list_status,num_episodes,start_season,broadcast,source,average_episode_duration,rating,pictures,background,related_anime,related_manga,studios,statistics");
            
            auto token = getTokenData();

            httphandler::HttpHeader header;
            header.name = "Authorization";
            header.value = "Bearer " + token.accessToken;
            std::vector<httphandler::HttpHeader> headers;
            headers.push_back(header);

            std::string urlstring = url.buildQueryString();

            httphandler::HttpResponse response = malhttphandler.get(urlstring, headers);
            if (response.statusCode != 200)
            {
                throw std::runtime_error("Failed to retrieve anime.");
            }

            json animeJson = json::parse(response.body);

            return animeJson;
    }

    json getUserAnimeList(){
        std::string baseURL ="https://api.myanimelist.net/v2/users/@me/animelist?";
        genericParameterBuilder url(baseURL);
        url.addParam("fields", "list_status");
        url.addParam("list_status", "limit=4");
        
        auto token = getTokenData();

        httphandler::HttpHeader header;
        header.name = "Authorization";
        header.value = "Bearer " + token.accessToken;
        std::vector<httphandler::HttpHeader> headers;
        headers.push_back(header);

        std::string urlstring = url.buildQueryString();

        httphandler::HttpResponse response = malhttphandler.get(urlstring, headers);
        if (response.statusCode != 200)
        {
            throw std::runtime_error("Failed to retrieve anime.");
        }

        json animeJson = json::parse(response.body);

        return animeJson;
    }


json getMangabyId(int id)
    {
            std::string baseURL ="https://api.myanimelist.net/v2/manga/" + std::to_string(id);
            genericParameterBuilder url(baseURL);
            url.addParam("fields", "id,title,main_picture,alternative_titles,start_date,end_date,synopsis,mean,rank,popularity,num_list_users,num_scoring_users,nsfw,created_at,updated_at,media_type,status,genres,my_list_status,num_volumes,num_chapters,authors{first_name,last_name},pictures,background,related_anime,related_manga,recommendations,serialization{name}");
            
            auto token = getTokenData();

            httphandler::HttpHeader header;
            header.name = "Authorization";
            header.value = "Bearer " + token.accessToken;
            std::vector<httphandler::HttpHeader> headers;
            headers.push_back(header);

            std::string urlstring = url.buildQueryString();

            httphandler::HttpResponse response = malhttphandler.get(urlstring, headers);
            if (response.statusCode != 200)
            {
                throw std::runtime_error("Failed to retrieve the managa.");
            }

            json animeJson = json::parse(response.body);

            return animeJson;
    }


json getUserAnimeList(const std::string& username)
    {
            std::string baseURL ="https://api.myanimelist.net/v2/users/" + username + "/animelist";
            genericParameterBuilder url(baseURL);
            url.addParam("fields", "limit");
            
            auto token = getTokenData();

            httphandler::HttpHeader header;
            header.name = "Authorization";
            header.value = "Bearer " + token.accessToken;
            std::vector<httphandler::HttpHeader> headers;
            headers.push_back(header);

            std::string urlstring = url.buildQueryString();

            httphandler::HttpResponse response = malhttphandler.get(urlstring, headers);
            //std::cout<<response.body<<std::endl;
            if (response.statusCode != 200)
            {
                throw std::runtime_error("Failed to retrieve the userslist.");
            }

            json animeJson = json::parse(response.body);

            return animeJson;
    }

json searchAnime(const std::string& searchquery){
    
    std::string baseurl = "https://api.myanimelist.net/v2/anime";
    genericParameterBuilder url(baseurl);
    auto token = getTokenData();

    httphandler::HttpHeader header;
    header.name = "Authorization";
    header.value = "Bearer " + token.accessToken;
    std::vector<httphandler::HttpHeader> headers;
    headers.push_back(header);

    url.addParam("q", searchquery);
    // url.addParam("limit", "1");         
    std::string querystringurl = url.buildQueryString();

    auto response = malhttphandler.get(querystringurl, headers);

    if(response.statusCode == 200){
        return json::parse(response.body);
    }else{
        return NULL;
    }
}

};