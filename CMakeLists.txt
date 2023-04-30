cmake_minimum_required(VERSION 3.23)

project(nav2)

add_library(nav2 INTERFACE)

target_include_directories(nav2 INTERFACE include)

enable_testing()
add_subdirectory(test)

# uncomment this to build examples
add_subdirectory(examples)