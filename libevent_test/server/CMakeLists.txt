CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Server)
SET(PROJECT_OUT Server)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)


INCLUDE_DIRECTORIES(
../../inc
../../inc/libevent)

SET(PROJECT_LIB_PATH ${PROJECT_SOURCE_DIR}/../../lib)
IF(WIN32)
	SET(PROJECT_LIBS_PLATFORM_DEBUG 
		${PROJECT_LIB_PATH}/event.lib
		${PROJECT_LIB_PATH}/event_core.lib
		${PROJECT_LIB_PATH}/event_extra.lib
		${PROJECT_LIB_PATH}/event_openssl.lib
		${PROJECT_LIB_PATH}/libcrypto.lib
		${PROJECT_LIB_PATH}/libssl.lib

		shell32 
		advapi32
		ws2_32.lib
		)

	SET(PROJECT_LIBS_PLATFORM_RELEASE
		${PROJECT_LIB_PATH}/event.lib
		${PROJECT_LIB_PATH}/event_core.lib
		${PROJECT_LIB_PATH}/event_extra.lib
		${PROJECT_LIB_PATH}/event_openssl.lib
		${PROJECT_LIB_PATH}/libcrypto.lib
		${PROJECT_LIB_PATH}/libssl.lib
		
		shell32 
		advapi32
		ws2_32.lib
		)

	#SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"LIBCMT.lib")
ELSEIF(APPLE)
	SET(PROJECT_LIBS_PLATFORM_DEBUG
			${PROJECT_LIB_PATH}/libevent.a
			${PROJECT_LIB_PATH}/libevent_core.a
			${PROJECT_LIB_PATH}/libevent_extra.a
			${PROJECT_LIB_PATH}/libcrypto.a
			${PROJECT_LIB_PATH}/libssl.a
			)
	SET(PROJECT_LIBS_PLATFORM_RELEASE
			${PROJECT_LIB_PATH}/libevent.a
			${PROJECT_LIB_PATH}/libevent_core.a
			${PROJECT_LIB_PATH}/libevent_extra.a
			${PROJECT_LIB_PATH}/libcrypto.a
			${PROJECT_LIB_PATH}/libssl.a
			)
ENDIF()
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DUNICODE)
SET(PROJECT_SOURCES main.cpp)

ADD_EXECUTABLE(${PROJECT_OUT} ${PROJECT_SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_OUT} ${PROJECT_LIBS_PLATFORM_RELEASE} ${PROJECT_LIBS_PLATFORM_DEBUG} )
