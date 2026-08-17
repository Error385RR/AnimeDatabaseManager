#pragma once
#include "animewebsiteapis.hpp"
#include <pugixml.hpp>


using json = nlohmann::json;


std::vector<int> animeids(const std::string& xmlfilepath){
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

void saveToFile(const std::string& filepathstring, const json& data){
    std::fstream filesaved;
    filesaved.open(filepathstring, std::ios_base::out);
    
    filesaved<<data.dump(3);

    filesaved.close();
}

void importer(const std::filesystem::path& xmlfilepath, const std::filesystem::path& rawfilesdirpath){
    auto ids = animeids(xmlfilepath);
    int count = 0;
    MalClient mal;

    for(const auto& i: ids){
        if(std::filesystem::exists(rawfilesdirpath.string() + std::to_string(i) + ".json")){
            continue;
        }
        auto jsonData = mal.getAnimebyId(i);

        saveToFile(rawfilesdirpath.string() + std::to_string(i) + ".json", jsonData);
        std::cout<<count<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(450));
        
        count++;
    }

}
