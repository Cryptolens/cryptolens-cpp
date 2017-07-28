cmake_minimum_required (VERSION 2.6)
project (skm_examples)

add_subdirectory ("SKM Client API")

add_executable(example1 example1.cpp)
target_link_libraries(example1 skm)
set_property(TARGET example1 PROPERTY CXX_STANDARD 11)
set_property(TARGET example1 PROPERTY CXX_STANDARD_REQURED ON)

add_executable(example2 example2.cpp)
target_link_libraries(example2 skm)
set_property(TARGET example2 PROPERTY CXX_STANDARD 11)
set_property(TARGET example2 PROPERTY CXX_STANDARD_REQURED ON)
