cmake_minimum_required(VERSION 3.20)

project(DungeonCrawler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib REQUIRED)

add_executable(dungeon_crawler
    src/main.cpp
    src/Player.cpp
    src/Room.cpp
)

target_include_directories(dungeon_crawler PRIVATE include)
target_link_libraries(dungeon_crawler PRIVATE raylib)