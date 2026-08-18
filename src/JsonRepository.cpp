#include "JsonRepository.hpp"


#include <vector>
#include "json.hpp"
#include <fstream>
#include <stdexcept>


std::string JsonRepository::stringNormalizer(const std::string& searchQuery)
{
    std::string result;
    bool previousWasSpace = false;
    for (unsigned char c : searchQuery)
    {
        if (std::isalnum(c)){
            result += static_cast<char>(std::tolower(c));
            previousWasSpace = false;
        }
        else if (std::isspace(c)){
            if (!result.empty() && !previousWasSpace){
                result += ' ';
                previousWasSpace = true;
            }
        }
        else{
            if (!result.empty() && !previousWasSpace){
                result += ' ';
                previousWasSpace = true;
            }
        }
    }
    if (!result.empty() && result.back() == ' ')
        result.pop_back();
    return result;
}




JsonRepository::JsonRepository(const std::filesystem::path& rawfilespath){
    if(std::filesystem::exists(rawfilespath) && std::filesystem::is_directory(rawfilespath)){
        directoryPath = rawfilespath;
    }else{
        throw std::runtime_error("Repository Directory does not exist");
    }
}
bool JsonRepository::animeExists(int id){
    std::string givenFilename = std::string(std::to_string(id) + ".json");
    auto filepath = directoryPath / givenFilename;
    return std::filesystem::exists(filepath);
}

anime::Anime JsonRepository::findById(int id){
    if(animeExists(id)){
        anime::Anime anime;
        json data;
        std::ifstream file(directoryPath / std::string(std::to_string(id) + ".json"), std::ios_base::in);
        if(!file.is_open()){
            throw std::runtime_error("Could Not Open " + (directoryPath / (std::to_string(id) + ".json")).string());
        }
        try {
                file >> data;
            }
            catch (const nlohmann::json::parse_error& e) {
                throw std::runtime_error(
                    "Invalid JSON in " +
                    (directoryPath / (std::to_string(id) + ".json")).string() +
                    ": " + e.what()
                );
            }
        anime = data;
        file.close();
        return anime;
    }else{
        throw std::runtime_error(std::string("Anime With " + std::to_string(id) + " Does Not Exist" ));
    }        
}
void JsonRepository::remove(int id){
    if(animeExists(id)){
        std::filesystem::remove(directoryPath / std::string(std::to_string(id) + ".json"));
        createIndex();
    }else{
        throw std::runtime_error("The Entry Does Not Exist");
    }
}
void JsonRepository::save(const anime::Anime& animest){
    if(!animeExists(animest.mal_id)){
        std::ofstream file(directoryPath / std::string(std::to_string(animest.mal_id) + ".json"));
        if(!file.is_open()){
            throw std::runtime_error("Could Not open the file to save the Anime with id " + (directoryPath / (std::to_string(animest.mal_id) + ".json")).string());
        }
        json data = animest;
        file<<data.dump(3);
        file.close();
        createIndex();
    }else{
        throw std::runtime_error("The Entry Already Exists");
    }
}

std::vector<anime::Anime> JsonRepository::getAll(){
    std::filesystem::directory_iterator dirit;
    try{
        dirit = std::filesystem::directory_iterator(directoryPath);
    }catch(const std::filesystem::filesystem_error& e){
            throw std::runtime_error("Could not access repository directory: " + std::string(e.what()));
    }
    std::vector<anime::Anime> allAnime;
    
    for(const auto& dirent: dirit){
        if (!dirent.is_regular_file() || dirent.path().extension() != ".json") continue;
        std::ifstream file(dirent.path());
        if(!file.is_open()){
            throw std::runtime_error("Could not open the file");
        }
        json data;
        try {
            file >> data;
        }
        catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error(
                "Invalid JSON in " + dirent.path().string() + ": " + e.what()
            );
        }
        anime::Anime anime = data;
        allAnime.push_back(anime);
        file.close();
    }
    return allAnime;
}

void JsonRepository::createIndex(){
    auto allAnime = getAll();
    std::ofstream file(directoryPath.parent_path() / "animeIndex.json");
    if(!file.is_open()){
        throw std::runtime_error("Could not open: " + (directoryPath.parent_path() / "animeIndex.json").string());
    }
    json dataArray = json::array();
    for(const auto& anime: allAnime){
        std::vector<std::string> synonyms;
        for (const auto& synonym : anime.alternativeTitles.synonyms){
            synonyms.push_back(stringNormalizer(synonym));
        }
        json data = {
            {"mal_id", anime.mal_id},
            {"normalized_title", stringNormalizer(anime.title)},
            {"title", anime.title},
            {"alt_titles", { 
                {"en", stringNormalizer(anime.alternativeTitles.english)},
                //{"ja", stringNormalizer(anime.alternativeTitles.japanese)},
                {"synonyms", synonyms}
            }}
        };
        dataArray.push_back(data);
    }
    file<<dataArray.dump(3);
    if (!file) throw std::runtime_error("Could not write anime index" + (directoryPath.parent_path() / "animeIndex.json").string());
    file.close();
}

json JsonRepository::loadIndexJson(){
    json indexJson;
    std::ifstream file(directoryPath.parent_path() / "animeIndex.json");
    if(!std::filesystem::exists(directoryPath.parent_path() / "animeIndex.json")){
        file.clear();
        createIndex();
        file.open(directoryPath.parent_path() / "animeIndex.json");
    }
    if(!file.is_open()) throw std::runtime_error("Could not open anime index at: " + (directoryPath.parent_path() / "animeIndex.json").string());
    try {
        file >> indexJson;
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error(
            "Invalid JSON in anime index: " + (directoryPath.parent_path() / "animeIndex.json").string() + ": " + e.what()
        );
    }
    file.close();
    return indexJson;
}
std::vector<JsonRepository::SearchResult> JsonRepository::searchAnimeByName(const std::string& query){
    std::string normalizedQuery = stringNormalizer(query);
    json indexjson = loadIndexJson();
    std::vector<SearchResult> results;
    
    for (const auto& entry : indexjson)
    {
        SearchResult entrystruct;
        // Required fields — malformed index if missing
        if (!entry.contains("mal_id") ||
            !entry.contains("title") ||
            !entry.contains("normalized_title"))
        {
            throw std::runtime_error("Malformed anime index entry");
        }
        if (entry["normalized_title"].get<std::string>().find(normalizedQuery) != std::string::npos)
        {
            entrystruct.title = entry["title"].get<std::string>();
            entrystruct.id = entry["mal_id"].get<int>();
            results.push_back(entrystruct);
        }else if (entry.contains("alt_titles"))
        {
            const auto& altTitles = entry["alt_titles"];

            if (altTitles.contains("en") &&
                altTitles["en"].get<std::string>().find(normalizedQuery) != std::string::npos)
            {
                entrystruct.title = entry["title"].get<std::string>();
                entrystruct.id = entry["mal_id"].get<int>();
                results.push_back(entrystruct);
            }
            else if (altTitles.contains("synonyms") &&
                    altTitles["synonyms"].is_array())
            {
                for (const auto& synonym : altTitles["synonyms"])
                {
                    if (synonym.is_string() &&
                        synonym.get<std::string>().find(normalizedQuery) != std::string::npos)
                    { 
                        entrystruct.title = entry["title"].get<std::string>();
                        entrystruct.id = entry["mal_id"].get<int>();
                        results.push_back(entrystruct);
                        break;
                    }
                }
            }
        }
    }
    return results;
}

