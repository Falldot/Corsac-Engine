cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(corsac VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)

include_directories(src)

file(GLOB SOURCES
    "src/*.c"
    "src/*/*.c"
)

add_subdirectory(test)

add_library(${PROJECT_NAME} STATIC ${SOURCES})