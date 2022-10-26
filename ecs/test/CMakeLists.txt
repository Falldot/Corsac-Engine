add_executable(ECS_Test "main_test.cpp")
include_directories(
        ./packages/Corsac_Test/include
)
add_subdirectory(./packages/Corsac_STL)
target_link_libraries(
        ECS_Test PRIVATE
        corsac::ECS
)
target_link_libraries(
        ECS_Test PRIVATE
        corsac::STL
)