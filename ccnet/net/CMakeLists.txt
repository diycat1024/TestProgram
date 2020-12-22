
project(server C CXX)

set(PROJECT_OUT server)
set(CXX_FLAGS 
        -g
        -std=c++11)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(PROJECT_SOURCES 
    server.cc
    Acceptor.cc
    EpollPoller.cc)
add_executable(${PROJECT_OUT} ${PROJECT_SOURCES})


