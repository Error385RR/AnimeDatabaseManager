# Header Files Documentation

This directory contains all the header files used in the Anime Database Manager project. These files define the interfaces for various components and provide declarations for classes, functions, and constants.

## File List and Purpose

### config.hpp
Contains configuration management classes and functions.
- Defines configuration data structures
- Handles loading and saving of application settings
- Manages paths for various application components

### http_client.hpp
Defines the HTTP client interface for making network requests.
- Declares HttpClient class methods for GET/POST operations
- Provides error handling and response parsing interfaces
- Contains HTTP request/response structures

### mal_api.hpp
Defines the MyAnimeList API client interface.
- Declares MalClient class methods for anime/manga data access
- Provides search, get by ID, and user list functionality
- Defines data structures for anime information

### oauth2.hpp
Contains OAuth2 authentication management classes.
- Declares OAuth2Client class for handling authentication flow
- Defines PKCE (Proof Key for Code Exchange) support
- Provides authorization URL generation and token exchange methods

### token_manager.hpp
Defines token management functionality.
- Declares TokenManager class for storing/retrieving tokens
- Handles token refresh logic when tokens expire
- Provides secure token storage mechanisms

### JsonRepository.hpp
Contains the JSON-based repository interface.
- Declares JsonRepository class for local data storage
- Defines CRUD operations for anime entries
- Provides search functionality with title normalization

### Library.hpp
Main application library interface.
- Declares Library class that orchestrates all components
- Defines public methods for database operations
- Provides high-level API for application functionality