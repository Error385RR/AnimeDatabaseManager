#include "AppPaths.hpp"

#include <cstdlib>
#include <stdexcept>

//namespace fs = std::filesystem;

std::filesystem::path AppPaths::homeDirectory()
{
#ifdef _WIN32

    const char* userProfile = std::getenv("USERPROFILE");

    if (!userProfile)
        throw std::runtime_error(
            "Unable to determine user home directory."
        );

    return fs::path(userProfile);

#else

    const char* home = std::getenv("HOME");

    if (!home)
        throw std::runtime_error(
            "Unable to determine user home directory."
        );

    return std::filesystem::path(home);

#endif
}

std::filesystem::path AppPaths::configDirectory()
{
    return homeDirectory() / "clidb" / "config";
}

std::filesystem::path AppPaths::configFile()
{
    return configDirectory() / "config.json";
}
