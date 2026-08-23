#include "Library.hpp"
#include "pugixml.hpp"

#include <iostream>
#include <algorithm>
#include <cctype>

Library::Library(IAnimeRepository& repository, IAnimeProvider& animeprovider)
        :repository(repository),
         animeProvider(animeprovider),
         repopath(repository.getRepoPath())
    {}

    std::vector<IAnimeRepository::SearchResult> Library::searchAnimelocal(const std::string& name){
        return repository.searchAnimeByName(name);
    }
    anime::Anime Library::getAnimeById(int id){
        return repository.findById(id);
    }
    anime::Anime Library::getAnimeByIdProvider(int id){
        return animeProvider.getAnimeById(id); 
    }

    IAnimeProvider::SearchPage Library::searchAnimeProvider(const std::string& name)
    {
        return animeProvider.searchAnimeByName(name);
    }
    
    void Library::savelocal(const anime::Anime& anime){
        repository.save(anime);
    }

    void Library::removelocal(int id){
        repository.remove(id);
    }
    void Library::refreshrepo(){
        repository.createIndex();
    }
    IAnimeProvider::SearchPage Library::getNextProviderSearchPage(const std::string& nexpageurl){
        return animeProvider.getSearchPage(nexpageurl);
    }

    std::vector<int> Library::animeids(const std::filesystem::path& xmlfilepath){
            std::filesystem::path xmlFilePath(xmlfilepath);
            pugi::xml_document doc;
            doc.load_file(xmlFilePath.c_str());
            auto root = doc.child("myanimelist");
            
            std::vector<int> ids;
            
            for(const auto i : root.children("anime")){
                ids.push_back(i.child("series_animedb_id").text().as_int());
            }

            return ids;
        }
    void Library::importer(const std::filesystem::path& xmlfilepath){
        auto ids = animeids(xmlfilepath);
        int count = 0;

        for(const auto& i: ids){
            try {
                std::cout << "Processing anime ID: " << i << '\n';
                if(repository.animeExists(i)){continue;}
                repository.save(animeProvider.getAnimeById(i));
                ++count;
                std::cout<< count << "- Successfully imported anime ID: " << i << '\n';
            }
            catch (const std::exception& e) {
                std::cerr
                    << "Failed to import anime "
                    << i
                    << ": "
                    << e.what()
                    << '\n'<<std::endl;
                continue;
            }
        }

}