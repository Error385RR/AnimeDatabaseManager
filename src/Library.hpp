#pragma once

#include <iostream>
#include <algorithm>
#include <cctype>
#include "JsonRepository.hpp"
#include "mal_api.hpp"
#include "pugixml.hpp"

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
    auto getAnimeById(int id){
        return repository.findById(id);
    }
    std::vector<IAnimeRepository::SearchResult> searchAnimeProvider(const std::string& name)
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
    void savelocal(const anime::Anime& anime){
        repository.save(anime);
    }

    void removelocal(int id){
        repository.remove(id);
    }
    void refreshrepo(){
        repository.createIndex();
    }


    std::vector<int> animeids(const std::filesystem::path& xmlfilepath){
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
    void importer(const std::filesystem::path& xmlfilepath, const std::filesystem::path& rawfilesdirpath){
        auto ids = animeids(xmlfilepath);
        int count = 0;

        for(const auto& i: ids){
            
            if(std::filesystem::exists(rawfilesdirpath /  std::filesystem::path(std::to_string(i) + ".json"))){
                continue;
            }
            ;
            auto jsonData = mal.getAnimebyId(i);
            std::cout << "Processing anime ID: " << i << '\n';
            try {
                std::cout << "Processing anime ID: " << i << '\n';

                auto jsonData = mal.getAnimebyId(i);
                repository.save(jsonData);
                std::filesystem::create_directories(rawfilesdirpath / "cache" / std::to_string(i));
                ++count;
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


};


