cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(SML VERSION 0.1.0)

include_directories(
    include
)

file(GLOB SOURCES "src/*.c" "src/*.cpp" "src/**/*.cpp" "src/**/*.c")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(
    corsac::STL PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
add_library(corsac::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
