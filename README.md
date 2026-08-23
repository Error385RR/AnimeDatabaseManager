# AnimeDatabaseManager

A local anime database manager using MAL exports and API data.

## Overview

This is a C++ command-line application that allows users to manage their personal anime collections by:
- Searching for anime locally or through MyAnimeList (MAL) API
- Adding anime to a local JSON-based database
- Deleting anime from the local database
- Synchronizing with MyAnimeList data using OAuth2 authentication

## Features

- Local anime database management
- MyAnimeList API integration with OAuth2 authentication
- Search functionality for both local and online anime
- Persistent storage using JSON files
- Command-line interface with menu-driven navigation

## Prerequisites

### System Requirements
- Windows 10 or later
- PowerShell 5.1 or later
- Visual Studio 2019 or later (with C++ development tools) OR MinGW/Clang compiler
- Internet connection for MyAnimeList API access

### Required Dependencies
- libcurl (for HTTP requests)
- nlohmann/json library (JSON parsing)
- httplib (HTTP server/client)
- pugixml (XML parsing)

## Installation

### 1. Clone the Repository
```powershell
git clone <repository-url>
cd cli_dbexp
```

### 2. Create Required Directory Structure
```powershell
# Create necessary directories for configuration and data storage
mkdir "clidb\config" 
mkdir "clidb\raw\anime"
```

### 3. Configure Application
Before running the application, you need to set up authentication with MyAnimeList:

1. Register your application at [MyAnimeList Developer Portal](https://myanimelist.net/apiconfig)
2. Create a `Malclient.txt` file in `clidb/config/` containing your MAL client ID
3. The first time you run the application, it will guide you through OAuth authentication

### 4. Building the Project

#### Using Visual Studio (Recommended):
1. Open `cli_dbexp.sln` (if available) or create a new project
2. Add all source files to the project
3. Configure dependencies:
   - Include paths for curl, json, httplib, pugixml
   - Linker settings for libcurl
4. Build the solution

#### Using CMake:
```powershell
mkdir build
cd build
cmake .. 
cmake --build .
```

## Usage

### Running the Application
```powershell
# From the build directory or where executable is located
.\cli_dbexp.exe
```

### Main Menu Options
1. **Add Anime** - Search for anime to add to your local database
2. **Search Anime** - Find anime in your local database or search online
3. **Delete Anime** - Remove anime from your local database
4. **Initialize Database** - Setup the local database structure
5. **Exit** - Close the application

### First-time Setup
1. Run the application for the first time
2. The application will prompt you to authenticate with MyAnimeList using OAuth2
3. Follow the authentication flow in your browser
4. After successful authentication, tokens will be saved for future use

## Project Structure

```
cli_dbexp/
├── src/                 # Source code files
│   ├── main.cpp         # Simple CLI entry point
│   ├── smallcli.cpp     # Menu-driven CLI entry point  
│   ├── config.cpp       # Configuration management
│   ├── http_client.cpp  # HTTP request handling
│   ├── mal_api.cpp      # MyAnimeList API integration
│   ├── oauth2.cpp       # OAuth2 authentication
│   ├── token_manager.cpp # Token management
│   ├── JsonRepository.cpp # Local JSON database
│   └── Library.cpp      # Main application logic
├── include/             # Header files
│   ├── config.hpp
│   ├── http_client.hpp
│   ├── mal_api.hpp
│   ├── oauth2.hpp
│   ├── token_manager.hpp
│   ├── JsonRepository.hpp
│   └── Library.hpp
├── clidb/               # Application data directory
│   ├── config/          # Configuration files
│   │   ├── Malclient.txt # MAL client ID
│   │   ├── Maltoken.json # OAuth tokens (generated)
│   │   └── config.json   # Main configuration
│   └── raw/             # Raw data storage
│       └── anime/       # Anime database files
├── COMPONENTS.md        # Component documentation
└── README.md            # This file
```

## Components

For detailed component documentation, see [COMPONENTS.md](COMPONENTS.md)

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- MyAnimeList API for providing anime data
- Open source libraries used in this project (libcurl, nlohmann/json, etc.)
