#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <optional>
#include <algorithm>
#include <filesystem>

#include "Library.hpp"

#include "config.hpp"
#include "JsonRepository.hpp"
#include "mal_api.hpp"
#include "Library.hpp"



using json = nlohmann::json;

enum class MenuOption
{
    AddAnime = 1,
    SearchAnime,
    DeleteAnime,
    InitializeDatabase,
    Exit
};

void displayMenu(
    const std::string& title,
    const std::vector<std::string>& menuItems
)
{
    std::cout << "\033[2J\033[1;1H";

    std::cout << title << "\n";

    for (const auto& item : menuItems)
    {
        std::cout << item << '\n';
    }

    std::cout << "Please select an option: ";
}

void clearInputBuffer()
{
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );
}

void waitforexit()
{
    clearInputBuffer();

    std::cout << "Press Any Key to return to the main menu...";

    std::cin.get();
}


/*
 * Display an anime's basic information.
 */
void displayAnime(const anime::Anime& animeData)
{
    std::vector<std::string> animeDetailsMenuItems = {
        "ID: " + std::to_string(animeData.mal_id),
        "Title: " + animeData.title,
        "Synopsis: " + animeData.synopsis,
        "Start Season: " + animeData.startSeason.season,
        "Start Year: " + std::to_string(animeData.startSeason.year),
        "Status: " + animeData.status,
        "Media Type: " + animeData.mediaType
    };

    displayMenu(
        "Anime Details",
        animeDetailsMenuItems
    );
}


/*
 * Select an anime from a provider search page.
 *
 * Returns the selected MAL ID.
 */
std::optional<int> selectProviderAnime(
    Library& app,
    IAnimeProvider::SearchPage& page
)
{
    while (true)
    {
        if (page.results.empty())
        {
            std::cout << "No results found.\n";

            if (page.nextPageURL.empty())
            {
                return std::nullopt;
            }
        }

        std::vector<std::string> menuItems;

        int count = 1;

        for (const auto& result : page.results)
        {
            menuItems.push_back(
                std::to_string(count++) +
                ". Title: " +
                result.title +
                ", ID: " +
                std::to_string(result.id)
            );
        }

        if (!page.nextPageURL.empty())
        {
            menuItems.push_back(
                std::to_string(count) +
                ". Next Page"
            );
        }

        menuItems.push_back(
            std::to_string(count + (!page.nextPageURL.empty() ? 1 : 0)) +
            ". Cancel"
        );

        displayMenu(
            "Provider Search Results",
            menuItems
        );

        std::cout << "Select an option: ";

        int selectedOption;

        if (!(std::cin >> selectedOption))
        {
            std::cin.clear();
            clearInputBuffer();

            std::cout << "Invalid selection.\n";
            continue;
        }

        if (
            selectedOption >= 1 &&
            selectedOption <= static_cast<int>(page.results.size())
        )
        {
            return page.results[selectedOption - 1].id;
        }

        int nextPageOption =
            static_cast<int>(page.results.size()) + 1;

        if (
            !page.nextPageURL.empty() &&
            selectedOption == nextPageOption
        )
        {
            try
            {
                page = app.getNextProviderSearchPage(
                    page.nextPageURL
                );
            }
            catch (const std::exception& e)
            {
                std::cerr
                    << "Failed to retrieve next page: "
                    << e.what()
                    << '\n';

                return std::nullopt;
            }

            continue;
        }

        int cancelOption =
            static_cast<int>(page.results.size()) +
            (!page.nextPageURL.empty() ? 2 : 1);

        if (selectedOption == cancelOption)
        {
            return std::nullopt;
        }

        std::cout << "Invalid selection.\n";
    }
}


/*
 * Search the local repository.
 */
void SearchAnime(Library& app)
{
    clearInputBuffer();

    std::string query;

    std::cout << "Enter the anime title to search: ";

    std::getline(std::cin, query);

    std::cout << "Searching locally for: "
              << query
              << '\n';

    auto result = app.searchAnimelocal(query);

    /*
     * No local results.
     *
     * Give the user the option to search the provider.
     */
    if (result.empty())
    {
        std::cout
            << "No local results found for: "
            << query
            << '\n';

        std::cout
            << "Would you like to search online? y/N: ";

        std::string choice;

        std::getline(std::cin, choice);

        if (choice != "y" && choice != "Y")
        {
            std::cout
                << "Returning to main menu."
                << std::endl;

            return;
        }

        /*
         * Provider search.
         */
        IAnimeProvider::SearchPage page;

        try
        {
            page = app.searchAnimeProvider(query);
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "Provider search failed: "
                << e.what()
                << '\n';

            return;
        }

        auto selectedID = selectProviderAnime(
            app,
            page
        );

        if (!selectedID)
        {
            return;
        }

        /*
         * Retrieve the complete anime from the provider.
         */
        anime::Anime animeData;

        try
        {
            animeData = app.getAnimeByIdProvider(
                *selectedID
            );
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "Failed to retrieve anime: "
                << e.what()
                << '\n';

            return;
        }

        displayAnime(animeData);

        std::cout
            << "Would you like to add this anime to "
               "your local database? (y/N): ";

        std::string addChoice;

        std::cin >> addChoice;

        if (
            addChoice == "y" ||
            addChoice == "Y"
        )
        {
            try
            {
                app.savelocal(animeData);

                std::cout
                    << "Anime added to local database."
                    << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cerr
                    << "Failed to save anime: "
                    << e.what()
                    << '\n';
            }
        }
        else
        {
            std::cout
                << "Anime not added to local database."
                << std::endl;
        }

        return;
    }


    /*
     * Local results exist.
     */
    std::vector<std::string> searchResultsMenuItems;

    int count = 1;

    for (const auto& i : result)
    {
        searchResultsMenuItems.push_back(
            std::to_string(count++) +
            ". Title: " +
            i.title +
            ", ID: " +
            std::to_string(i.id)
        );
    }

    displayMenu(
        "Local Search Results",
        searchResultsMenuItems
    );

    std::cout
        << "Select the anime to view: ";

    int selectedOption;

    if (!(std::cin >> selectedOption))
    {
        std::cin.clear();
        clearInputBuffer();

        std::cout
            << "Invalid selection."
            << std::endl;

        return;
    }

    if (
        selectedOption < 1 ||
        selectedOption > static_cast<int>(result.size())
    )
    {
        std::cout
            << "Invalid selection. "
               "Returning to main menu."
            << std::endl;

        return;
    }

    try
    {
        anime::Anime animeData =
            app.getAnimeById(
                result[selectedOption - 1].id
            );

        displayAnime(animeData);
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Failed to retrieve anime: "
            << e.what()
            << '\n';
    }
}


/*
 * Add an anime directly from the provider.
 */
void addAnime(Library& app)
{
    std::cout << "\033[2J\033[1;1H";

    clearInputBuffer();

    std::string title;

    std::cout
        << "Enter the title of the anime to add: ";

    std::getline(
        std::cin,
        title
    );

    IAnimeProvider::SearchPage page;

    try
    {
        page = app.searchAnimeProvider(title);
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Provider search failed: "
            << e.what()
            << '\n';

        return;
    }

    auto selectedID =
        selectProviderAnime(
            app,
            page
        );

    if (!selectedID)
    {
        return;
    }

    anime::Anime animeData;

    try
    {
        animeData =
            app.getAnimeByIdProvider(
                *selectedID
            );
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Failed to retrieve anime: "
            << e.what()
            << '\n';

        return;
    }

    displayAnime(animeData);

    std::cout
        << "Add this anime to the local database? "
           "(y/N): ";

    std::string choice;

    std::cin >> choice;

    if (
        choice == "y" ||
        choice == "Y"
    )
    {
        try
        {
            app.savelocal(animeData);

            std::cout
                << "Anime added successfully."
                << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "Failed to save anime: "
                << e.what()
                << '\n';
        }
    }
}


/*
 * Delete an anime from the local repository.
 */
void deleteAnime(Library& app)
{
    std::cout << "\033[2J\033[1;1H";

    clearInputBuffer();

    std::string title;

    std::cout
        << "Enter the title of the anime to delete: ";

    std::getline(
        std::cin,
        title
    );

    auto result =
        app.searchAnimelocal(title);

    if (result.empty())
    {
        std::cout
            << "No anime found."
            << std::endl;

        return;
    }

    std::vector<std::string>
        deleteAnimeMenuItems;

    int count = 1;

    for (const auto& i : result)
    {
        deleteAnimeMenuItems.push_back(
            std::to_string(count++) +
            ". Title: " +
            i.title +
            ", ID: " +
            std::to_string(i.id)
        );
    }

    displayMenu(
        "Delete Anime",
        deleteAnimeMenuItems
    );

    std::cout
        << "Select the anime to delete: ";

    int selectedOption;

    if (!(std::cin >> selectedOption))
    {
        std::cin.clear();
        clearInputBuffer();

        std::cout
            << "Invalid selection."
            << std::endl;

        return;
    }

    if (
        selectedOption < 1 ||
        selectedOption > static_cast<int>(result.size())
    )
    {
        std::cout
            << "Invalid selection."
            << std::endl;

        return;
    }

    try
    {
        app.removelocal(
            result[selectedOption - 1].id
        );

        std::cout
            << "Anime removed successfully."
            << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Failed to remove anime: "
            << e.what()
            << '\n';
    }
}


/*
 * Import a MyAnimeList XML export.
 */
void importFromXML(Library& app)
{
    clearInputBuffer();

    std::string xmlfilepathstring;

    std::cout
        << "Enter the XML File Path: ";

    std::getline(
        std::cin,
        xmlfilepathstring
    );

    std::filesystem::path xmlfile(
        xmlfilepathstring
    );

    if (
        !std::filesystem::exists(xmlfile)
    )
    {
        displayMenu(
            "Import Anime",
            {
                "Could not open the XML file.",
                "Please check that the file exists."
            }
        );

        return;
    }

    try
    {
        app.importer(xmlfile);
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Import failed: "
            << e.what()
            << '\n';
    }
}


int main()
{
    Config configManager("config/config.json");

    Config::configdata cfg =
        configManager.loadConfig();

    /*
     * Create the concrete implementations.
     */
    JsonRepository jsonRepo(
        std::filesystem::path(
            cfg.jsonrepositorypath
        )
    );

    MalProvider malProvider(cfg);

    /*
     * Inject both dependencies into Library.
     */
    Library app(
        jsonRepo,
        malProvider
    );

    std::vector<std::string> mainmenuitems = {
        "1. Add Anime",
        "2. Search Anime",
        "3. Delete Anime",
        "4. Import",
        "5. Exit"
    };

    while (true)
    {
        displayMenu(
            "Main Menu",
            mainmenuitems
        );

        char menuSelection;

        std::cin.get(menuSelection);

        switch (menuSelection)
        {
            case '1':
                addAnime(app);
                waitforexit();
                break;

            case '2':
                SearchAnime(app);
                waitforexit();
                break;

            case '3':
                deleteAnime(app);
                waitforexit();
                break;

            case '4':
                importFromXML(app);
                waitforexit();
                break;

            case '5':
                std::cout
                    << "Exiting the application.\n";

                return 0;

            default:
                std::cout
                    << "Invalid selection."
                    << std::endl;

                waitforexit();
                break;
        }
    }

    return 0;
}