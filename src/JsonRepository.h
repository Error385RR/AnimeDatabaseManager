#include <cassert>
#include <iostream>
#include <vector>
#include <optional>
#include "/home/error/codework/cpp/cli_dbexp/src/json.hpp"
#include <fstream>
#include <stdexcept>
#include <utility>

using json = nlohmann::json;
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


namespace anime{
    struct start_season{
        std::string season;
        int year;
    };

    struct Studio{
        int id;
        std::string studioName;
    };

    struct broadcast{
        std::string dayoftheWeek;
        std::string start_time;
    };

    struct Genre{
        int id;
        std::string genreName;
    };

    struct Image{
        std::string large;
        std::string medium;
    };
    struct AnimeReference{
        int id;
        Image referencedMainPicture;
        std::string title;
    };
    struct related_anime{
        AnimeReference referencedAnime;
        std::string relation_type;
    };
    struct related_manga{
        AnimeReference referencedManga;
        std::string relation_type;
    };    
    struct AlternativeTitles{
        std::string english;
        std::string japanese;
        std::vector<std::string> synonyms;
    };

    struct Anime{
        int mal_id;
        std::string title;
        std::string synopsis;
        std::string source;
        std::string status;
        std::string mediaType;
        std::string startDate;
        std::string endDate;
        std::string background;
        std::string createdAt;
        std::string updatedAt;
        std::string nsfw;
        int averageEpisodeDurationSec;
        int popularity;
        double mean;
        int numEpisodes;
        start_season startSeason;
        std::vector<Studio> studios;
        broadcast broadcasts;
        std::vector<Genre> genres;    
        Image thumbnail;
        std::vector<Image> pictures;
        AlternativeTitles alternativeTitles;
        std::vector<related_anime> related_animes;
        std::vector<related_manga> related_mangas;
};

    void from_json(const json& j, Genre& genre)
    {
        genre.id = j.at("id").get<int>();
        genre.genreName = j.at("name").get<std::string>();
    }
    void from_json(const json& j, Studio& studio){
        studio.id = j.at("id").get<int>();
        studio.studioName = j.at("name").get<std::string>();
    }
    void from_json(const json& j, Image& image){
        image.large = j.at("large").get<std::string>();
        image.medium = j.at("medium").get<std::string>();
    }
    void from_json(const json& j, AnimeReference& animerefrence){
        animerefrence.id = j.at("id").get<int>();
        animerefrence.referencedMainPicture = j.at("main_picture").get<Image>();
        animerefrence.title = j.at("title").get<std::string>();
    }   
    void from_json(const json& j, broadcast& broadcast){
            broadcast.dayoftheWeek = j.at("day_of_the_week").get<std::string>();
            broadcast.start_time = j.at("start_time").get<std::string>();
        }
    void from_json(const json& j, start_season& startSeason){
        startSeason.season = j.at("season").get<std::string>();
        startSeason.year = j.at("year").get<int>();
    }
    void from_json(const json& j, AlternativeTitles& alternativeTitles){
        alternativeTitles.english = j.at("en").get<std::string>();
        alternativeTitles.japanese = j.at("ja").get<std::string>();
        alternativeTitles.synonyms = j.at("synonyms").get<std::vector<std::string>>();
    }
    void from_json(const json& j, related_anime& relatedAnime){
        relatedAnime.referencedAnime = j.at("node").get<AnimeReference>();
        relatedAnime.relation_type = j.at("relation_type").get<std::string>();
    }
    void from_json(const json& j, related_manga& relatedManga){
        relatedManga.referencedManga = j.at("related_manga").get<AnimeReference>();
        relatedManga.relation_type = j.at("relation_type").get<std::string>();
    }
    void from_json(const json& j, Anime& anime){
        anime.mal_id = j.at("id").get<int>();
        anime.title = j.at("title").get<std::string>();
        anime.synopsis = j.at("synopsis").get<std::string>();
        anime.source = j.at("source").get<std::string>();
        anime.status = j.at("status").get<std::string>();
        anime.mediaType = j.at("media_type").get<std::string>();
        anime.startDate = j.at("start_date").get<std::string>();
        anime.endDate = j.at("end_date").get<std::string>();
        anime.background = j.at("background").get<std::string>();
        anime.createdAt = j.at("created_at").get<std::string>();
        anime.updatedAt = j.at("updated_at").get<std::string>();
        anime.nsfw = j.at("nsfw").get<std::string>();
        anime.averageEpisodeDurationSec = j.at("average_episode_duration").get<int>();
        anime.popularity = j.at("popularity").get<int>();
        anime.mean = j.at("mean").get<double>();
        anime.numEpisodes = j.at("num_episodes").get<int>();
        anime.startSeason = j.at("start_season").get<start_season>();
        anime.studios = j.at("studios").get<std::vector<Studio>>();
        // anime.broadcasts = j.at("broadcast").get<broadcast>();
        anime.genres = j.at("genres").get<std::vector<Genre>>();
        anime.thumbnail = j.at("main_picture").get<Image>();
        anime.pictures = j.at("pictures").get<std::vector<Image>>();
        anime.alternativeTitles = j.at("alternative_titles").get<AlternativeTitles>();
        anime.related_animes = j.at("related_anime").get<std::vector<related_anime>>();
        anime.related_mangas = j.at("related_manga").get<std::vector<related_manga>>();
    }
    void to_json(json& j, const start_season& season)
    {
        j = json{
            {"season", season.season},
            {"year", season.year}
        };
    }
    void to_json(json& j, const Studio& studio)
    {
        j = json{
            {"id", studio.id},
            {"name", studio.studioName}
        };
    }
    void to_json(json& j, const broadcast& broadcast)
    {
        j = json{
            {"day_of_the_week", broadcast.dayoftheWeek},
            {"start_time", broadcast.start_time}
        };
    }
    void to_json(json& j, const Genre& genre)
    {
        j = json{
            {"id", genre.id},
            {"name", genre.genreName}
        };
    }
    void to_json(json& j, const Image& image)
    {
        j = json{
            {"large", image.large},
            {"medium", image.medium}
        };
    }
    void to_json(json& j, const AnimeReference& reference)
    {
        j = json{
            {"id", reference.id},
            {"main_picture", reference.referencedMainPicture},
            {"title", reference.title}
        };
    }
    void to_json(json& j, const related_anime& relatedAnime)
    {
        j = json{
            {"node", relatedAnime.referencedAnime},
            {"relation_type", relatedAnime.relation_type}
        };
    }
    void to_json(json& j, const related_manga& relatedManga)
    {
        j = json{
            {"node", relatedManga.referencedManga},
            {"relation_type", relatedManga.relation_type}
        };
    }
    void to_json(json& j, const AlternativeTitles& titles)
    {
        j = json{
            {"en", titles.english},
            {"ja", titles.japanese},
            {"synonyms", titles.synonyms}
        };
    }
    void to_json(json& j, const Anime& anime)
    {
        j = json{
            {"id", anime.mal_id},
            {"title", anime.title},
            {"synopsis", anime.synopsis},
            {"source", anime.source},
            {"status", anime.status},
            {"media_type", anime.mediaType},
            {"start_date", anime.startDate},
            {"end_date", anime.endDate},
            {"background", anime.background},
            {"created_at", anime.createdAt},
            {"updated_at", anime.updatedAt},
            {"nsfw", anime.nsfw},
            {"average_episode_duration", anime.averageEpisodeDurationSec},
            {"popularity", anime.popularity},
            {"mean", anime.mean},
            {"num_episodes", anime.numEpisodes},
            {"start_season", anime.startSeason},
            {"studios", anime.studios},
            {"broadcast", anime.broadcasts},
            {"genres", anime.genres},
            {"main_picture", anime.thumbnail},
            {"pictures", anime.pictures},
            {"alternative_titles", anime.alternativeTitles},
            {"related_anime", anime.related_animes},
            {"related_manga", anime.related_mangas}
        };
    }

}
class JsonRepository:IAnimeRepository{
private:
    std::string stringNormalizer(const std::string& searchQuery)
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

public:
    std::filesystem::path directoryPath;

    JsonRepository(const std::filesystem::path& rawfilespath){
        if(std::filesystem::exists(rawfilespath) && std::filesystem::is_directory(rawfilespath)){
            directoryPath = rawfilespath;
        }else{
            throw std::runtime_error("Repository Directory does not exist");
        }
    }
    bool animeExists(int id){
        std::string givenFilename = std::string(std::to_string(id) + ".json");
        auto filepath = directoryPath / givenFilename;
        return std::filesystem::exists(filepath);
    }

    anime::Anime findById(int id){
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
    void remove(int id){
        if(animeExists(id)){
            std::filesystem::remove(directoryPath / std::string(std::to_string(id) + ".json"));
            createIndexJson();
        }else{
            throw std::runtime_error("The Entry Does Not Exist");
        }
    }
    void save(const anime::Anime& animest){
        if(!animeExists(animest.mal_id)){
            std::ofstream file(directoryPath / std::string(std::to_string(animest.mal_id) + ".json"));
            if(!file.is_open()){
                throw std::runtime_error("Could Not open the file to save the Anime with id " + (directoryPath / (std::to_string(animest.mal_id) + ".json")).string());
            }
            json data = animest;
            file<<data.dump(3);
            file.close();
            createIndexJson();
        }else{
            throw std::runtime_error("The Entry Already Exists");
        }
    }

    std::vector<anime::Anime> getAll(){
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

    void createIndexJson(){
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
    
    auto loadIndexJson(){
        json indexJson;
        std::ifstream file(directoryPath.parent_path() / "animeIndex.json");
        if(!std::filesystem::exists(directoryPath.parent_path() / "animeIndex.json")){
            file.clear();
            createIndexJson();
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
    std::vector<std::pair<std::string, int>> searchAnimebyName(const std::string& query){
        std::string normalizedQuery = stringNormalizer(query);
        json indexjson = loadIndexJson();
        std::vector<std::pair<std::string, int>> results;
        
        for (const auto& entry : indexjson)
        {
            std::pair<std::string, int> entrypair;
            // Required fields — malformed index if missing
            if (!entry.contains("mal_id") ||
                !entry.contains("title") ||
                !entry.contains("normalized_title"))
            {
                throw std::runtime_error("Malformed anime index entry");
            }
            if (entry["normalized_title"].get<std::string>().find(normalizedQuery) != std::string::npos)
            {
                entrypair = {
                    entry["title"].get<std::string>(),
                    entry["mal_id"].get<int>()
                };
                results.push_back(entrypair);
            }else if (entry.contains("alt_titles"))
            {
                const auto& altTitles = entry["alt_titles"];

                if (altTitles.contains("en") &&
                    altTitles["en"].get<std::string>().find(normalizedQuery) != std::string::npos)
                {
                    entrypair = {
                        entry["title"].get<std::string>(),
                        entry["mal_id"].get<int>()
                    };

                    results.push_back(entrypair);
                }
                else if (altTitles.contains("synonyms") &&
                        altTitles["synonyms"].is_array())
                {
                    for (const auto& synonym : altTitles["synonyms"])
                    {
                        if (synonym.is_string() &&
                            synonym.get<std::string>().find(normalizedQuery) != std::string::npos)
                        {
                            entrypair = {
                                synonym.get<std::string>(),
                                entry["mal_id"].get<int>()
                            };

                            results.push_back(entrypair);
                            break;
                        }
                    }
                }
            }
        }
        return results;
    }

};


// int main()
// {
//     std::string rawfilespath = "/home/error/codework/cpp/cli_dbexp/raw/anime";
//     std::string query;
//     JsonRepository repo(rawfilespath);
//     repo.createIndexJson();
//     std::cout<<"Search for: ";
//     getline(std::cin, query);
//     auto results = repo.searchAnimebyName(query);
//     for(const auto& i: results){
//         std::cout<<i.second<<std::endl;
//     }
//     return 0;
// }

