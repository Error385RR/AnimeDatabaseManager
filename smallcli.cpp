#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <optional>
#include <algorithm>
#include "config.hpp"
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
    std::cout << "Press Any Key to return to the main menu...";
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
    if(searchResultsMenuItems.empty())
    {
        std::cout << "No results found for: " << query << std::endl;
        std::cout<<"would you like to search online? y/N"<<std::endl;
        std::string choice;
        std::getline(std::cin, choice);
        if(choice == "y" || choice == "Y")
        {
            result = app.searchAnimeProvider(query);
            searchResultsMenuItems.clear();
            count = 1;
            for (const auto& i : result)
            {
                searchResultsMenuItems.push_back(std::to_string(count++) + ". Title: " + i.title + ", ID: " + std::to_string(i.id));
            }
            displayMenu("Search Results", searchResultsMenuItems);
            std::cout << "Select the anime to add by entering the corresponding number: ";
            int selectedOption;
            std::cin >> selectedOption; // Convert to zero-based index
            if(selectedOption < 1 || selectedOption > static_cast<int>(result.size()))
            {
                std::cout << "Invalid selection. Returning to main menu." << std::endl;
                return;
            }
            anime::Anime animeData = app.getAnimeByIdProvider(result[selectedOption - 1].id);
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
            std::cout<< "Would you like to add this anime to your local database? (y/N): ";
            std::string addChoice;
            std::cin >> addChoice;
            if(addChoice == "y" || addChoice == "Y")
            {
                app.savelocal(animeData);
                std::cout << "Anime added to local database." << std::endl;
            }
            else
            {
                std::cout << "Anime not added to local database." << std::endl;
            }
            return;
        }
        else
        {
            std::cout << "Returning to main menu." << std::endl;
            return;
        }
    }
    displayMenu("Search Results", searchResultsMenuItems);
    std::cout << "Select the anime to add by entering the corresponding number: ";
    int selectedOption;
    std::cin >> selectedOption; 
    if(selectedOption < 1 || selectedOption > static_cast<int>(result.size()))
    {
        std::cout << "Invalid selection. Returning to main menu." << std::endl;
        return;
    }
    anime::Anime animeData = app.getAnimeById(result[selectedOption - 1].id);
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
    int selectedOption;
    std::cin >> selectedOption; 
    if(selectedOption < 1 || selectedOption > static_cast<int>(result.size()))
    {
        std::cout << "Invalid selection. Returning to main menu." << std::endl;
        return;
    }
    anime::Anime animeData = app.getAnimeByIdProvider(result[selectedOption - 1].id);
    json jsonData = animeData;
    app.savelocal(animeData);

}
void deleteAnime(Library& app)
{
    std::cout << "\033[2J\033[1;1H";
    clearInputBuffer();
    std::string title;
    std::cout << "Enter the title of the anime to delete: ";
    std::getline(std::cin, title);
    // Implement the logic to delete anime here
    auto result = app.searchAnimelocal(title);
    std::vector<std::string> deleteAnimeMenuItems;
    int count = 1;
    for (const auto& i : result)
    {
        deleteAnimeMenuItems.push_back(std::to_string(count++) + ". Title: " + i.title + ", ID: " + std::to_string(i.id));
    }
    displayMenu("Delete Anime", deleteAnimeMenuItems);
    std::cout << "Select the anime to delete by entering the corresponding number: ";
    int selectedOption;
    std::cin >> selectedOption; 
    if(selectedOption < 1 || selectedOption > static_cast<int>(result.size()))
    {
        std::cout << "Invalid selection. Returning to main menu." << std::endl;
        return;
    }
    app.removelocal(result[selectedOption - 1].id);
}

void importFromXML(Library& app){
    std::string xmlfilepathstring;
    clearInputBuffer();
    std::cout<<"Enter the XML File Path: ";
    std::getline(std::cin, xmlfilepathstring);

    std::filesystem::path xmlfile(xmlfilepathstring);
    if(!std::filesystem::exists(xmlfile)){
        displayMenu("Import Anime", {"Could Not Open The XML file, please check if the file is valid or exists"});
        return;
    }

    app.importer(xmlfile);
}

int main()
{   
    Config configManager;
    Config::configdata cfg = configManager.loadConfig();
    std::cout<<cfg.clientidfilepath<<std::endl;
    std::cout<<cfg.MALTOKENFilePath<<std::endl;
    std::cout<<cfg.jsonFilePath<<std::endl;
    std::cout<<cfg.jsonrepositorypath<<std::endl;
    
    JsonRepository jsonRepo(std::filesystem::path(cfg.jsonrepositorypath));
    Library app(jsonRepo);
    char menuSelection;
    std::vector<std::string> mainmenuitems = {
        "1. Add Anime",
        "2. Search Anime",
        "3. Delete Anime",
        "4. Import",
        // "5. Initialize Database",
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
            deleteAnime(app);
            waitforexit();
            goto MAINMENU;
            break;
        case '4':
            menuSelection = '4';
            importFromXML(app);
            goto MAINMENU;
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