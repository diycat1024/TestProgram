CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(LT)
SET(PROJECT_OUTNAME LT)

IF(WIN32)
    add_definitions(-DWIN32)
ELSEIF(APPLE)
    add_definitions(-DMACOS)
ENDIF()
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(server)
add_subdirectory(client)

