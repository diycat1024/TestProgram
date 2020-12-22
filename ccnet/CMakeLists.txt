
cmake_minimum_required(version 2.8)
project(ccnet C CXX)

set(PROJECT_OUT ccnet)
set(CXX_FLAGS 
        -g
        -std=c++11)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(example)