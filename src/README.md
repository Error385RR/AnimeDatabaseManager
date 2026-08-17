# MyAnimeList OAuth2 Client Library

This is a C++ library for authenticating with the MyAnimeList API using OAuth2 with PKCE support.

## Overview

The library provides a complete implementation for:
- OAuth2 authentication flow with PKCE
- HTTP request handling
- Token management (storage and refresh)
- MyAnimeList API integration

## Files Structure

- `oauth.hpp` - Main header file that includes all components
- `pkce.hpp` - PKCE code verifier and challenge generation
- `http_client.hpp` - HTTP client wrapper using libcurl
- `token_manager.hpp` - Token persistence and validation
- `oauth2.hpp` - Core OAuth2 implementation
- `mal_api.hpp` - MyAnimeList specific API client

## Usage

Include the main header file in your project:

```cpp
#include "src/oauth.hpp"

// Use the MalClient class for full MAL API access
MalClient mal_client;
auto token = mal_client.getTokenData();

// Get anime information
auto anime_data = mal_client.getAnimebyId(12345);
```

## Requirements

- C++17 or later
- libcurl development libraries
- OpenSSL development libraries
- httplib.h (included in this repository)
- json.hpp (included in this repository)

## Build Instructions

Compile with:

```bash
g++ -std=c++17 -o myapp main.cpp -lcurl -lssl -lcrypto
```

## License

MIT License