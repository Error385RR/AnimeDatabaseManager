#pragma once
#include <iostream>
#include <vector>
#include "Models.hpp"

class IAnimeRepository {
public:
    virtual ~IAnimeRepository() = default;
    struct SearchResult{
        int id;
        std::string title;
    };
;
    virtual void save(const anime::Anime&) = 0;
    virtual anime::Anime findById(int id) = 0;
    virtual void remove(int id) = 0;
    virtual bool animeExists(int id) = 0;
    virtual void createIndex() = 0;
    virtual std::vector<anime::Anime> getAll() = 0;
    virtual std::vector<SearchResult> searchAnimeByName(const std::string&) = 0;
};