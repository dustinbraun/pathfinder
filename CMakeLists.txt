cmake_minimum_required(VERSION 3.23)

project(nav2)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(nav2 INTERFACE)

target_include_directories(nav2 INTERFACE include)

add_executable(main src/main.cpp)

target_link_libraries(main ${SDL2_LIBRARIES} nav2)

enable_testing()

add_subdirectory(test)
