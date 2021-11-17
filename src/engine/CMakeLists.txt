cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(corsac_engine VERSION 0.1.0)

include_directories(
    include
        ../SML/include
        ../STL/include
)

file(GLOB SOURCES "main.cpp" "src/*.cpp" "src/*.c" "src/**/*.cpp" "src/**/*.c")

add_executable(corsac_engine WIN32 ${SOURCES})
set_target_properties(
    corsac_engine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    WIN32_EXECUTABLE FALSE
)
target_link_libraries(corsac_engine corsac::SML)
target_link_libraries(corsac_engine corsac::STL)

