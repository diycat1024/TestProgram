cmake_minimum_required(VERSION 2.8)
project(muduo_chat_server)
set(PROJECT_OUT muduo_chat_server)

set(CMAKE_CXX_STANDARD 11)

set(muduo_root "/aseit-data/work_zhang/cppCode/muduo")
include_directories(${muduo_root}/)
link_directories(${muduo_root}/../build/release-cpp11/lib/)


set(PROJECT_SOURCE chat_server.cc)
set(PROJECT_LIBRARY muduo_base
    muduo_net
    pthread)


add_executable(${PROJECT_OUT} ${PROJECT_SOURCE})
target_link_libraries(${PROJECT_OUT} ${PROJECT_LIBRARY})

add_executable(muduo_chat_client chat_client.cc)
target_link_libraries(muduo_chat_client ${PROJECT_LIBRARY})
