#pragma once
#include <iostream>
#include <vector>
#include "Models.hpp"

class IAnimeRepository {
public:
    virtual ~IAnimeRepository() = default;

    virtual void save(const anime::Anime&) = 0;
    virtual anime::Anime findById(int id) = 0;
    virtual void remove(int id) = 0;
    virtual bool animeExists(int id) = 0;
    virtual std::vector<anime::Anime> getAll() = 0;
    virtual std::vector<std::pair<std::string, int>>
        searchAnimeByName(const std::string&) = 0;
};