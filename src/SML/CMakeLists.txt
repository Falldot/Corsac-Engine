cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(SML VERSION 0.1.0)

set(NAME_SPACE "corsac")

find_package(SDL2 REQUIRED)

include_directories(
    include
)

file(GLOB SOURCES "src/*.c" "src/*.cpp" "src/**/*.cpp" "src/**/*.c")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
add_library(${NAME_SPACE}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
        ${NAME_SPACE}::STL
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_image
)
