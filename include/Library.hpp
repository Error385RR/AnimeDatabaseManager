#pragma once

#include "IAnimeRepository.hpp"
#include "IAnimeProvider.hpp"

class Library{
private:
    IAnimeRepository& repository;
    IAnimeProvider& animeProvider;
    
    std::filesystem::path repopath;
public:
    explicit Library(IAnimeRepository& repository, IAnimeProvider& animeprovider);
    
    // Local repository operations
    std::vector<IAnimeRepository::SearchResult> searchAnimelocal(const std::string& name);
    anime::Anime getAnimeById(int id);
    void savelocal(const anime::Anime& anime);
    void removelocal(int id);
    void refreshrepo();
    
    //Provider Operations
    anime::Anime getAnimeByIdProvider(int id);
    IAnimeProvider::SearchPage searchAnimeProvider(const std::string& name);
    IAnimeProvider::SearchPage getNextProviderSearchPage(const std::string& nexpageurl); 

    // Import
    std::vector<int> animeids(const std::filesystem::path& xmlfilepath);
    void importer(const std::filesystem::path& xmlfilepath);


};


