#pragma once

#include <filesystem>
#include <string>

#include "json.hpp"

class Config
{
public:

    struct configdata
    {
        std::filesystem::path clientidfilepath;
        std::filesystem::path MALTOKENFilePath;
        std::filesystem::path jsonFilePath;
        std::filesystem::path jsonrepositorypath;
    };

    Config();

    explicit Config(
          const std::filesystem::path& configFilePath
      );

    configdata getDefaults() const;

    bool createConfig(
          const configdata& defaults
      ) const;

    configdata loadConfig();

private:

    std::filesystem::path configPath;
};
