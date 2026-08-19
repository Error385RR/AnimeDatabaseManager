#include "Library.hpp"
#include "pugixml.hpp"

#include <iostream>
#include <algorithm>
#include <cctype>

Library::Library(IAnimeRepository& repository)
        :repository(repository)
    {}

    std::vector<IAnimeRepository::SearchResult> Library::searchAnimelocal(const std::string& name){
        return repository.searchAnimeByName(name);
    }
    anime::Anime Library::getAnimeById(int id){
        return repository.findById(id);
    }
    anime::Anime Library::getAnimeByIdProvider(int id){
        return mal.getAnimebyId(id); 
    }

    std::vector<IAnimeRepository::SearchResult> Library::searchAnimeProvider(const std::string& name)
    {
        auto data = mal.searchAnime(name).at("data");

        std::vector<IAnimeRepository::SearchResult> results;

        for (const auto& entry : data)
        {
            const auto& node = entry.at("node");

            results.push_back({
                node.at("id").get<int>(),
                node.at("title").get<std::string>()
            });
        }

        return results;
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
    void Library::importer(const std::filesystem::path& xmlfilepath, const std::filesystem::path& rawfilesdirpath){
        auto ids = animeids(xmlfilepath);
        int count = 0;

        for(const auto& i: ids){
            
            if(std::filesystem::exists(rawfilesdirpath /  std::filesystem::path(std::to_string(i) + ".json"))){
                continue;
            }
            try {
                std::cout << "Processing anime ID: " << i << '\n';

                auto jsonData = mal.getAnimebyId(i);
                repository.save(jsonData);
                std::filesystem::create_directories(rawfilesdirpath / "cache" / std::to_string(i));
                ++count;
                std::cout<< count << "- Successfully imported anime ID: " << i << '\n';
            }
            catch (const std::exception& e) {
                std::cerr
                    << "Failed to import anime "
                    << i
                    << ": "
                    << e.what()
                    << '\n';

                continue;
            }
        }

}