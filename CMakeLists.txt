# GEMINI GENERATED FILE JUST FOR LANGUAGE TESTING
cmake_minimum_required(VERSION 3.15)
project(GULAG VERSION 2.0 LANGUAGES CXX)

# Enforce C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disables compiler-specific extensions for strict compliance

include_directories(${CMAKE_SOURCE_DIR}/src/utils)

add_executable(test_lang
    src/test/test_lang.cpp
    src/utils/lang.cpp
)

add_executable(test_config
    src/test/test_config.cpp
    src/utils/config.cpp
)

target_compile_options(test_lang PRIVATE -Wall -Wextra -Wpedantic -Werror)
