CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(HiRedisTest)
SET(PROJECT_OUTNAME HiRedisTest)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-D_WIN32_WINNT=0x0501)
#add_definitions(-DCMAKE_TOOLCHAIN_FILE="E:/idzqj/Work/harry/IM-Server/vcpkg/scripts/buildsystems/vcpkg.cmake")
 SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"MSVCRT.lib")

 SET(ThirdParty "E:/idzqj/Work/harry/IM-Server/vcpkg/installed/x64-windows")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


message("PROJECT_SOURCE_DIR" ${PROJECT_SOURCE_DIR})
#设置头文件路径
INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}/../inc
                ${PROJECT_SOURCE_DIR}/inc
                ${ThirdParty}/include
)
# 设置库目录
link_directories(
        ${PROJECT_SOURCE_DIR}/../lib
        ${ThirdParty}/lib
)
message("thirdyparty:"  ${PROJECT_SOURCE_DIR}/../lib)

SET(PROJECT_LIBS_PLATFORM_DEBUG
        hiredis.lib
    )
SET(PROJECT_LIBS_PLATFORM_RELEASE
        ws2_32.lib
        hiredis.lib
)

# 设置项目
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEAD_FILES ${PROJECT_SOURCE_DIR}/inc/*.h)

ADD_EXECUTABLE(${PROJECT_OUTNAME} ${SRC_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_OUTNAME}  ${PROJECT_LIBS_PLATFORM_RELEASE}  )
MESSAGE(STATUS "${SRC_FILES} is SRC_FILES dir:" ${SRC_FILES})
MESSAGE(STATUS "${HEAD_FILES} is HEAD_FILES dir:" ${HEAD_FILES})
