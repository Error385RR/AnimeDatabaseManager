#pragma once
#include <iostream>
#include <vector>
#include "Models.hpp"

class IAnimeProvider {
public:
    virtual ~IAnimeProvider() = default;
    struct SearchResult{
        int id;
        std::string title;
        // std::vector<std::string> thumbnail;
    };
    struct SearchPage{
        std::vector<SearchResult> results;
        std::string nextPageURL;
    };
    virtual SearchPage getSearchPage(const std::string& nextPageURL) = 0;
    virtual anime::Anime getAnimeById(int id) = 0;
    virtual SearchPage searchAnimeByName(const std::string&) = 0;

};