cmake_minimum_required (VERSION 2.6)
project (skm_tests)

add_subdirectory (googletest/googletest)

add_executable(tests tests.cpp)
target_link_libraries(tests skm gtest curl crypto ssl)
