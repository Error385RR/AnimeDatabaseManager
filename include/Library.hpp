#pragma once

#include "JsonRepository.hpp"
#include "mal_api.hpp"


class Library{
private:
    IAnimeRepository& repository;
    MalClient mal = MalClient(Config::getDefaults());
public:
    explicit Library(IAnimeRepository& repository);
    std::vector<IAnimeRepository::SearchResult> searchAnimelocal(const std::string& name);
    anime::Anime getAnimeById(int id);
    std::vector<IAnimeRepository::SearchResult> searchAnimeProvider(const std::string& name);
    
    void savelocal(const anime::Anime& anime);
    void removelocal(int id);
    void refreshrepo();
    anime::Anime getAnimeByIdProvider(int id);

    std::vector<int> animeids(const std::filesystem::path& xmlfilepath);
    void importer(const std::filesystem::path& xmlfilepath, const std::filesystem::path& rawfilesdirpath);


};


