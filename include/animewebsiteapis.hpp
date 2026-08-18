#pragma once

// Main OAuth2 library header that includes all components
// This file provides a complete interface for MyAnimeList OAuth2 authentication and API access

#include "pkce.hpp"
#include "http_client.hpp"
#include "token_manager.hpp"
#include "oauth2.hpp"
#include "mal_api.hpp"

// This header file acts as a convenient way to include all OAuth2 functionality
// Users can simply include this one file to get access to the complete library