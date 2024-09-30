cmake_minimum_required(VERSION 3.29)
project(map_api_cpp)

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add nlohmann/json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(map_api_cpp main.cpp)

target_link_libraries(map_api_cpp PRIVATE CURL::libcurl nlohmann_json::nlohmann_json sfml-graphics sfml-window sfml-system)
