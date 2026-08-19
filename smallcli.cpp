#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <optional>
#include <algorithm>
#include "Library.hpp"

enum class MenuOption
{
    AddAnime = 1,
    SearchAnime,
    DeleteAnime,
    initializeDatabase,
    Exit
};

void displayMenu(const std::string& title, const std::vector<std::string>& menuItems)
{
    std::cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left
    std::cout << title << "\n";
    for (const auto& item : menuItems)
    {
        std::cout << item << '\n';
    }
    std::cout << "Please select an option: ";
}

void clearInputBuffer()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void waitforexit()
{
    clearInputBuffer();
    std::cout << "Press Enter to return to the main menu...";
    std::cin.get();
}

void SearchAnime(Library& app)
{
    clearInputBuffer();
    std::string query;
    std::cout << "Enter the anime title to search: ";
    std::getline(std::cin, query);
    // Implement the search logic here
    std::cout << "Searching for: " << query << std::endl;
    auto result = app.searchAnimelocal(query);
    // Process the search result
    std::vector<std::string> searchResultsMenuItems;
    int count = 1;
    for (const auto& i : result)
    {
        searchResultsMenuItems.push_back(std::to_string(count++) + ". Title: " + i.title + ", ID: " + std::to_string(i.id));
    }
    displayMenu("Search Results", searchResultsMenuItems);
    std::cout << "Select the anime to add by entering the corresponding number: ";
    char selectedOption;
    std::cin.get(selectedOption);
    int selectedIndex = selectedOption - '1'; // Convert char to index
    anime::Anime animeData = app.getAnimeById(result[selectedIndex].id);
    json jsonData = animeData;
    std::vector<std::string> animeDetailsMenuItems = {
        "ID: " + std::to_string(animeData.mal_id),
        "Title: " + animeData.title,
        "Synopsis: " + animeData.synopsis,
        "Start Season: " + animeData.startSeason.season,
        "Start Year: " + std::to_string(animeData.startSeason.year),
        "Status: " + animeData.status,
        "Media Type: " + animeData.mediaType
    };
    displayMenu("Anime Details", animeDetailsMenuItems);
                                    

    
}

void addAnime(Library& app)
{
    std::cout << "\033[2J\033[1;1H";
    clearInputBuffer();
    std::string title;
    std::cout << "Enter the title of the anime to add: ";
    std::getline(std::cin, title);
    // Implement the logic to add anime here
    auto result = app.searchAnimeProvider(title);
    std::vector<std::string> addAnimeMenuItems;
    int count = 1;
    for (const auto& i : result)
    {
        addAnimeMenuItems.push_back(std::to_string(count++) + ". Title: " + i.title + ", ID: " + std::to_string(i.id));
    }
    displayMenu("Add Anime", addAnimeMenuItems);
    std::cout << "Select the anime to add by entering the corresponding number: ";
    char selectedOption;
    std::cin.get(selectedOption);
    int selectedIndex = selectedOption - '1'; // Convert char to index
    anime::Anime animeData = app.getAnimeByIdProvider(result[selectedIndex].id);
    json jsonData = animeData;
    app.savelocal(animeData);

}
int main()
{   
    JsonRepository jsonRepo(std::filesystem::path("D:/Dev/Projects/cli_dbexp/raw/anime"));
    Library app(jsonRepo);
    char menuSelection;
    std::vector<std::string> mainmenuitems = {
        "1. Add Anime",
        "2. Search Anime",
        "3. Delete Anime",
        "4. Initialize Database",
        "5. Exit"
    };
    MAINMENU:
    
    displayMenu("Main Menu", mainmenuitems);
    std::cin.get(menuSelection); 
    switch(menuSelection)
    {
        case '1':
            menuSelection = '1';
            addAnime(app);
            waitforexit();
            goto MAINMENU;
            break;
        case '2':
            SearchAnime(app);
            waitforexit();
            goto MAINMENU;
            break;
        case '3':
            menuSelection = '3';
            break;
        case '4':
            menuSelection = '4';
            break;
        case '5':
            std::cout << "Exiting the application.\n";
            break;
        default:
            std::cout << "Invalid selection. Please try again.\n";
            waitforexit();
            goto MAINMENU;
    }



    return 0;
}