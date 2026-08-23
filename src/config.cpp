#include "config.hpp"

Config::Config(const std::filesystem::path& configFilePath)
    : configPath(configFilePath)
{}


Config::configdata Config::getDefaults()
    {
        return {
            "clidb/config/Malclient.txt",
            "clidb/config/Maltoken.json",
            "clidb/config/config.json",
            "clidb/raw/anime"
        };
    }
    bool Config::createConfig(const configdata& defaults)
    {
        std::filesystem::create_directories(configPath.parent_path()
        );

        std::ofstream configFile(configPath);

        if (!configFile)
        {
            std::cerr << "Failed to create config file.\n";
            return false;
        }

        json configJson = {
            {"clientidfilepath", defaults.clientidfilepath.string()},
            {"MALTOKENFilePath", defaults.MALTOKENFilePath.string()},
            {"jsonFilePath", defaults.jsonFilePath.string()},
            {"jsonrepositorypath", defaults.jsonrepositorypath.string()}
        };

        configFile << configJson.dump(4);

        return true;
    }

    Config::configdata Config::loadConfig()
    {

        // Config doesn't exist
        if (!std::filesystem::exists(configPath))
        {
            if (!createConfig(defaults))
            {
                throw std::runtime_error(
                    "Configuration file does not exist and could not be created."
                );
            }

            return defaults;
        }
        json configJson;

        try
        {
            std::ifstream configFile(configPath);
            configFile >> configJson;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Malformed config file: "
                    << e.what()
                    << "\nRepairing configuration...\n";
            if (!createConfig(defaults))
            {
                throw std::runtime_error(
                    "Configuration was malformed and could not be repaired."
                );
            }

            return defaults;
        }

        bool repaired = false;

        // Repair individual values
        if (!configJson.contains("clientidfilepath") ||
            !configJson["clientidfilepath"].is_string())
        {
            configJson["clientidfilepath"] =
                defaults.clientidfilepath.string();

            repaired = true;
        }

        if (!configJson.contains("MALTOKENFilePath") ||
            !configJson["MALTOKENFilePath"].is_string())
        {
            configJson["MALTOKENFilePath"] =
                defaults.MALTOKENFilePath.string();

            repaired = true;
        }

        if (!configJson.contains("jsonFilePath") ||
            !configJson["jsonFilePath"].is_string())
        {
            configJson["jsonFilePath"] =
                defaults.jsonFilePath.string();

            repaired = true;
        }

        if (!configJson.contains("jsonrepositorypath") ||
            !configJson["jsonrepositorypath"].is_string())
        {
            configJson["jsonrepositorypath"] =
                defaults.jsonrepositorypath.string();

            repaired = true;
        }

        // Save repaired configuration
        if (repaired)
        {
            std::ofstream configFile(configPath);

            if (!configFile)
                throw std::runtime_error(
                    "Failed to save repaired configuration."
                );

            configFile << configJson.dump(4);
        }

        // Convert JSON → configdata
        configdata cfg;

        cfg.clientidfilepath =
            configJson["clientidfilepath"].get<std::string>();

        cfg.MALTOKENFilePath =
            configJson["MALTOKENFilePath"].get<std::string>();

        cfg.jsonFilePath =
            configJson["jsonFilePath"].get<std::string>();

        cfg.jsonrepositorypath =
            configJson["jsonrepositorypath"].get<std::string>();

        return cfg;
    }