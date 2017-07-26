cmake_minimum_required (VERSION 2.6)
project (skm_examples)

add_subdirectory ("SKM Client API")

add_executable(example1 example1.cpp)
target_link_libraries(example1 skm curl crypto ssl)

add_executable(example2 example2.cpp)
target_link_libraries(example2 skm curl crypto ssl)
