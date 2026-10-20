cmake_minimum_required(VERSION 3.16)
project(snake_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(snake_client src/client_main.cpp src/ui/terminal.cpp src/ui/input.cpp)
target_include_directories(snake_client PRIVATE include)
