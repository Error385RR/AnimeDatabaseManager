#include <iostream>
#include "JsonRepository.h"
#include "mal_api.hpp"

class Library{
private:
    IAnimeRepository& repository;
    MalClient mal;
public:
    explicit Library(IAnimeRepository& repository)
        :repository(repository)
    {}

    auto searchAnimelocal(const std::string& name){
        return repository.searchAnimeByName(name);
    }

    auto searchAnimeprovider(const std::string& name){
        return mal.searchAnime(name);
    }

    void savelocal(const anime::Anime& anime){
        repository.save(anime);
    }

    void removelocal(int id){
        repository.remove(id);
    }

};

