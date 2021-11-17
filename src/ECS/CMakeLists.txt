cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ECS VERSION 0.1.0 LANGUAGES CXX)
set(NAME_SPACE "corsac")

option(${PROJECT_NAME}_TESTING "Включить модульное тестирование" ON)

if (MSVC)
    add_compile_options(
        -Wall
    )
else()
    add_compile_options(
        -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
        -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal
        -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual
        -Wredundant-decls -Wsign-conversion -Wsign-promo
    )
endif()

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(NOT ${PROJECT_NAME}_TESTING)
    message(STATUS "Тестирование выключено!")
else()
    add_subdirectory(test)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
add_library(${NAME_SPACE}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

