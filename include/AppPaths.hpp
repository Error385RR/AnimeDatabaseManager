#pragma once

#include <filesystem>

class AppPaths
{
public:
    static std::filesystem::path homeDirectory();
    static std::filesystem::path configDirectory();
    static std::filesystem::path configFile();
};
