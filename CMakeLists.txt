cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(Corsac VERSION 0.1.0 LANGUAGES CXX)

if (MSVC)
    add_compile_options(/W4)
else()
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
endif()

# if(NOT CMAKE_CXX_EXTENSIONS)
#     set(CMAKE_CXX_EXTENSIONS OFF)
# endif()

add_subdirectory(src/ecs)
add_subdirectory(src/app)
add_subdirectory(src/engine)

# enable_testing()

# file(GLOB ECS "src/ecs/test/main.cpp" "src/ecs/test/*.cpp" "src/ecs/test/**/.cpp")
# add_executable(corsac_ecs_test ${ECS})
# target_include_directories(
#     corsac_ecs_test
#     PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/include
# )
# target_link_libraries(corsac_ecs_test corsac_ecs)