#pragma once

#include "Models.hpp"
#include "IAnimeRepository.hpp"



class JsonRepository: public IAnimeRepository{
private:
    static std::string stringNormalizer(const std::string& searchQuery);
    std::filesystem::path directoryPath;

public:

    explicit JsonRepository(const std::filesystem::path& rawfilespath);
    bool animeExists(int id)override;
    anime::Anime findById(int id) override;
    void save(const anime::Anime& animest)override;
    void remove(int id)override;

    std::vector<anime::Anime> getAll() override;
    void createIndex() override;
    std::filesystem::path getRepoPath() override;
    json loadIndexJson();
    std::vector<SearchResult> searchAnimeByName(const std::string& query) override;
};
