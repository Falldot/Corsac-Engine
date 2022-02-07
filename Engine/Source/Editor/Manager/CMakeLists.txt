cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(corsac VERSION 0.1.0 LANGUAGES CXX)

#if (MSVC)
#    add_compile_options(
#            -Wall
#    )
#else()
#    add_compile_options(
#            -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
#            -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal
#            -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
#            -Wredundant-decls -Wsign-conversion -Wsign-promo
#    )
#endif()

add_executable(${PROJECT_NAME} manager.cpp)
set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
