#pragma once

#include <iostream>
#include "json.hpp"

using json = nlohmann::json;



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

    inline void from_json(const json& j, Genre& genre)
    {
        genre.id = j.at("id").get<int>();
        genre.genreName = j.at("name").get<std::string>();
    }
    inline void from_json(const json& j, Studio& studio){
        studio.id = j.at("id").get<int>();
        studio.studioName = j.at("name").get<std::string>();
    }
    inline void from_json(const json& j, Image& image){
        image.large = j.at("large").get<std::string>();
        image.medium = j.at("medium").get<std::string>();
    }
    inline void from_json(const json& j, AnimeReference& animerefrence){
        animerefrence.id = j.at("id").get<int>();
        animerefrence.referencedMainPicture = j.at("main_picture").get<Image>();
        animerefrence.title = j.at("title").get<std::string>();
    }   
    inline void from_json(const json& j, broadcast& broadcast){
            broadcast.dayoftheWeek = j.at("day_of_the_week").get<std::string>();
            broadcast.start_time = j.at("start_time").get<std::string>();
        }
    inline void from_json(const json& j, start_season& startSeason){
        startSeason.season = j.at("season").get<std::string>();
        startSeason.year = j.at("year").get<int>();
    }
    inline void from_json(const json& j, AlternativeTitles& alternativeTitles){
        alternativeTitles.english = j.at("en").get<std::string>();
        alternativeTitles.japanese = j.at("ja").get<std::string>();
        alternativeTitles.synonyms = j.at("synonyms").get<std::vector<std::string>>();
    }
    inline void from_json(const json& j, related_anime& relatedAnime){
        relatedAnime.referencedAnime = j.at("node").get<AnimeReference>();
        relatedAnime.relation_type = j.at("relation_type").get<std::string>();
    }
    inline void from_json(const json& j, related_manga& relatedManga){
        relatedManga.referencedManga = j.at("related_manga").get<AnimeReference>();
        relatedManga.relation_type = j.at("relation_type").get<std::string>();
    }
    inline void from_json(const json& j, Anime& anime){
        anime.mal_id = j.at("id").get<int>();
        anime.title = j.at("title").get<std::string>();
        anime.synopsis = j.at("synopsis").get<std::string>();
        anime.source = j.at("source").get<std::string>();
        anime.status = j.at("status").get<std::string>();
        anime.mediaType = j.at("media_type").get<std::string>();
        anime.startDate = j.at("start_date").get<std::string>();
        anime.endDate = j.at("end_date").get<std::string>();
        anime.background = j.value("background", std::string{});
        anime.createdAt = j.at("created_at").get<std::string>();
        anime.updatedAt = j.at("updated_at").get<std::string>();
        anime.nsfw = j.at("nsfw").get<std::string>();
        anime.averageEpisodeDurationSec = j.at("average_episode_duration").get<int>();
        anime.popularity = j.value("popularity", 0);
        anime.mean = j.value("mean", 0.0);
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
    inline void to_json(json& j, const start_season& season)
    {
        j = json{
            {"season", season.season},
            {"year", season.year}
        };
    }
    inline void to_json(json& j, const Studio& studio)
    {
        j = json{
            {"id", studio.id},
            {"name", studio.studioName}
        };
    }
    inline void to_json(json& j, const broadcast& broadcast)
    {
        j = json{
            {"day_of_the_week", broadcast.dayoftheWeek},
            {"start_time", broadcast.start_time}
        };
    }
    inline void to_json(json& j, const Genre& genre)
    {
        j = json{
            {"id", genre.id},
            {"name", genre.genreName}
        };
    }
    inline void to_json(json& j, const Image& image)
    {
        j = json{
            {"large", image.large},
            {"medium", image.medium}
        };
    }
    inline void to_json(json& j, const AnimeReference& reference)
    {
        j = json{
            {"id", reference.id},
            {"main_picture", reference.referencedMainPicture},
            {"title", reference.title}
        };
    }
    inline void to_json(json& j, const related_anime& relatedAnime)
    {
        j = json{
            {"node", relatedAnime.referencedAnime},
            {"relation_type", relatedAnime.relation_type}
        };
    }
    inline void to_json(json& j, const related_manga& relatedManga)
    {
        j = json{
            {"node", relatedManga.referencedManga},
            {"relation_type", relatedManga.relation_type}
        };
    }
    inline void to_json(json& j, const AlternativeTitles& titles)
    {
        j = json{
            {"en", titles.english},
            {"ja", titles.japanese},
            {"synonyms", titles.synonyms}
        };
    }
    inline void to_json(json& j, const Anime& anime)
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
