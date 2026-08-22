# Anime Database Manager - Component Documentation

## Overview
This document describes the core components of the Anime Database Manager application, a C++ command-line tool that manages local anime databases using MyAnimeList API integration.

## Components

### 1. OAuth2Client
Handles the complete OAuth2 authentication flow with PKCE support for accessing MyAnimeList API.
- Uses libcurl for HTTP requests
- Manages authorization URLs and token exchange process
- Implements proper error handling and token validation

### 2. TokenManager
Manages persistent storage of OAuth tokens for seamless API access.
- Handles token persistence and refresh logic when tokens expire
- Provides functionality to load, save, and refresh tokens
- Ensures secure handling of access and refresh tokens

### 3. HttpClient
Wrapper around libcurl for making HTTP requests to MyAnimeList API.
- Supports both GET and POST operations
- Handles response parsing and error management with proper cleanup
- Provides a consistent interface for all HTTP interactions

### 4. JsonRepository
Local JSON-based storage system for anime data.
- Provides CRUD operations (create, read, update, delete) for anime entries
- Includes search functionality with normalized titles for efficient lookups
- Stores data in structured JSON format for easy persistence and retrieval

### 5. MalClient
MyAnimeList API client that provides methods for fetching anime and manga data.
- Provides methods for getting anime/manga by ID
- Includes user list retrieval, search functionality, and individual ID-based lookups
- Integrates with TokenManager for authentication and HttpClient for HTTP operations

## Architecture Overview

┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   MalClient     │    │  TokenManager   │    │   HttpClient    │
│                 │    │                 │    │                 │
│ - API Methods   │───▶│ - Token Storage │───▶│ - HTTP Requests │
│ - Search        │    │ - Refresh Logic │    │ - Response Parse│
│ - Get by ID     │    │ - Load/Save     │    │ - Error Handling│
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                    ┌─────────────────────────────┐
                    │   JsonRepository            │
                    │                             │
                    │ - Local Storage             │
                    │ - Search Functionality      │
                    │ - CRUD Operations           │
                    └─────────────────────────────┘

## Data Flow

1. User interacts with the CLI application (smallcli.cpp or main.cpp)
2. Application uses MalClient to access MyAnimeList API
3. MalClient authenticates using TokenManager 
4. Authentication is handled by OAuth2Client which uses HttpClient for HTTP operations
5. API responses are processed and stored in JsonRepository
6. Local searches use JsonRepository directly
7. Tokens are managed by TokenManager with automatic refresh when needed