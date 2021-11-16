add_executable(STL_Test "test_main.cpp")
target_link_libraries(
        STL_Test PRIVATE
        corsac::STL
)