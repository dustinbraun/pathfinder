cmake_minimum_required(VERSION 3.23)

project(pathfinder)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


add_library(pathfinder INTERFACE)

target_include_directories(pathfinder INTERFACE include)

add_executable(main src/main.cpp)

target_link_libraries(main ${SDL2_LIBRARIES} pathfinder)

add_subdirectory(test)
