CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(TestSET)
SET(PROJECT_OUTNAME TestProgramSet)
add_definitions(-DUNICODE)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#add_subdirectory(mongo-cxx-test)
#add_subdirectory(acl-redis-test)
#add_subdirectory(hiredis_test)
#add_subdirectory(rpc_test)
#add_subdirectory(mong-c-test)
add_subdirectory(libevent_test)

MESSAGE(STATUS "${PROJECT_OUTNAME} is BINARY dir:" ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "${PROJECT_OUTNAME} is SOURCE dir:" ${PROJECT_SOURCE_DIR})

