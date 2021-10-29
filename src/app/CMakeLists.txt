cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(corsac_app VERSION 0.1.0)

include_directories(
    include
)

file(GLOB SOURCES "src/*.c" "src/*.cpp" "src/**/*.cpp" "src/**/*.c")

add_library(corsac_app STATIC ${SOURCES})
set_target_properties(
    corsac_ecs PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
add_library(corsac::corsac_app ALIAS corsac_app)
