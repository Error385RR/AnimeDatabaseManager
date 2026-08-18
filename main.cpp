#include <iostream>
#include <filesystem>
#include <fstream>
#include "json.hpp"
#include "Library.hpp"


int main(){
    JsonRepository jsonRepo(std::filesystem::path("D:/Dev/Projects/cli_dbexp/raw/anime"));
    Library app(jsonRepo);
    std::string query;
    std::getline(std::cin, query);
    bool didnotfind = true;
    std::string input;
    auto result = app.searchAnimelocal(query);
    if(!result.empty()){
        for(const auto& i: result){
            std::cout<<i.title<<std::endl;
            std::cout<<i.id<<std::endl;
        }
        std::getline(std::cin, input);
        if(input == "q") didnotfind = false;
    }else if(result.empty() || didnotfind == true){
        result = app.searchAnimeProvider(query);
        for(const auto& i : result){
            std::cout<<i.title<<std::endl;
        }
    }
    return 0;
}