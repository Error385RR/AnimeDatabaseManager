#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "json.hpp"

using json = nlohmann::json;

class Config{
private:
    std::filesystem::path configPath;

public:

    struct configdata
    {
        std::filesystem::path clientidfilepath;
        std::filesystem::path MALTOKENFilePath;
        std::filesystem::path jsonFilePath;
        std::filesystem::path jsonrepositorypath;
    };

    Config(const std::filesystem::path& configFilePath);
    configdata defaults = getDefaults();

    static configdata getDefaults();
    bool createConfig(const configdata& defaults);

    configdata loadConfig();
};



