add_executable(ECS_Test "test_main.cpp")
target_link_libraries(
        ECS_Test PRIVATE
        corsac::STL
)