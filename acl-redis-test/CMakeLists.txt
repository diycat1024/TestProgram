CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(AclRedisTest)
SET(PROJECT_OUTNAME AclRedisTest)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Md")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

message("PROJECT_SOURCE_DIR" ${PROJECT_SOURCE_DIR})
#设置头文件路径
INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}/../inc
        ${PROJECT_SOURCE_DIR}/inc
        "E:/idzqj/Work/harry/IM-Server/im_server_mongodb/inc/AntThirdParty"
        "E:/idzqj/Work/harry/IM-Server/vcpkg/installed/x64-windows/include"
)
# 设置库目录
link_directories(${PROJECT_SOURCE_DIR}/../lib)


SET(PROJECT_LIBS_PLATFORM_DEBUG
        lib_acl_vc2017d
        lib_acl_cpp_vc2017d
    )
SET(PROJECT_LIBS_PLATFORM_RELEASE
        lib_acl_vc2017d
        lib_acl_cpp_vc2017
)

# 设置项目
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEAD_FILES ${PROJECT_SOURCE_DIR}/inc/*.h)

ADD_EXECUTABLE(${PROJECT_OUTNAME} ${SRC_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_OUTNAME}  ${PROJECT_LIBS_PLATFORM_DEBUG} ${PROJECT_LIBS_PLATFORM_RELEASE} )
MESSAGE(STATUS "${SRC_FILES} is SRC_FILES dir:" ${SRC_FILES})
MESSAGE(STATUS "${HEAD_FILES} is HEAD_FILES dir:" ${HEAD_FILES})
