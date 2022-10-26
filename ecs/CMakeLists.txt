cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ECS VERSION 0.1.0 LANGUAGES CXX)
set(NAME_SPACE "corsac")
set(CMAKE_CXX_STANDARD 17)

option(${PROJECT_NAME}_TESTING "Включить модульное тестирование" ON)

if (MSVC)
    add_compile_options(
            -Wall
    )
else()
    add_compile_options(
            -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
            -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal
            -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
            -Wredundant-decls -Wsign-conversion -Wsign-promo -O3
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
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
add_library(${NAME_SPACE}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} INTERFACE ${NAME_SPACE}::STL)