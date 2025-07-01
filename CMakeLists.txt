cmake_minimum_required(VERSION 3.16)

# Project name and version
project(CryptoCourse VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable debug symbols for development
set(CMAKE_BUILD_TYPE Debug)

# Add compiler flags for better warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Create executable
add_executable(crypto_course src/main.cpp)

# Include directories
target_include_directories(crypto_course PRIVATE include)
