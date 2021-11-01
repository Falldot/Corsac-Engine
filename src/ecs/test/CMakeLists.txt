cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(corsac_ecs_test LANGUAGES CXX)


include_directories(
    ../ecs/include
)

file(GLOB SOURCES "test_main.cpp")

enable_testing()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} corsac_ecs)
add_test(ECS ${PROJECT_NAME})
# add_custom_command(TARGET corsac_ecs_test
#                 POST_BUILD
#                 COMMAND ctest -C Debug --output-on-failure)
#target_sources(corsac_ecs_test PRIVATE mylib/myfeature.cpp)

