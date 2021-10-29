cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(corsac_ecs_test LANGUAGES CXX)

enable_testing()

file(GLOB SOURCES "test_main.cpp" "*.cpp" "**/*.cpp" )

add_executable(corsac_ecs_test ${SOURCES})
target_link_libraries(corsac_ecs_test
    PRIVATE
        corsac::corsac_ecs
)
add_test(ECS corsac_ecs_test)
add_custom_command(TARGET corsac_ecs_test
                POST_BUILD
                COMMAND ctest -C Debug --output-on-failure)
#target_sources(corsac_ecs_test PRIVATE mylib/myfeature.cpp)

