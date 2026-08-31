cmake_minimum_required(VERSION 4.0)

project(cli_db
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(OpenSSL 3 REQUIRED)

add_executable(clidb
    smallcli.cpp
    src/Library.cpp
    src/JsonRepository.cpp
    src/mal_api.cpp
    src/oauth2.cpp
    src/token_manager.cpp
    src/http_client.cpp
    src/parameterbuilder.cpp
    src/config.cpp
    src/AppPaths.cpp
)

target_include_directories(clidb PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(clidb PRIVATE
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    pugixml
#    ws2_32
)
