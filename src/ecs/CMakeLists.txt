cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(corsac_ecs VERSION 0.1.0 LANGUAGES CXX)

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

option(CORSAC_ECS_TESTING "Включить модульное тестирование" ON)

include_directories(
    include
)

file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp")

if(NOT CORSAC_ECS_TESTING)
    message(STATUS "Тестирование выключено")
else()
    add_subdirectory(test)
endif()

add_library(corsac_ecs STATIC ${SOURCES})
set_target_properties(
    corsac_ecs PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
add_library(corsac::corsac_ecs ALIAS corsac_ecs)
