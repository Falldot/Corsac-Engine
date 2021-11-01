cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(corsac_ecs VERSION 0.1.0 LANGUAGES CXX)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

include_directories(
    include
        include/internal
)

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp")

add_subdirectory(test)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(
        ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
add_library(corsac::corsac_ecs ALIAS ${PROJECT_NAME})

