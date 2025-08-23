cmake_minimum_required(VERSION 3.14)
project(nodescm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        node_addon_api
        GIT_REPOSITORY https://github.com/nodejs/node-addon-api.git
        GIT_TAG main
)
FetchContent_MakeAvailable(node_addon_api)


# Try to use node-gyp headers if available
if(DEFINED ENV{USERPROFILE})
        set(NODE_GYP_INCLUDE "$ENV{USERPROFILE}/.node-gyp/${NODE_VERSION}/include/node")
        if(EXISTS "${NODE_GYP_INCLUDE}/node_api.h")
                set(NODE_INCLUDE_DIR "${NODE_GYP_INCLUDE}")
        else()
                set(NODE_INCLUDE_DIR "src")
        endif()
else()
        set(NODE_INCLUDE_DIR "src")
endif()

add_library(nodescm SHARED bindings.cpp)
set_target_properties(nodescm PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(nodescm
        PRIVATE
        ${node_addon_api_SOURCE_DIR}
        ${NODE_INCLUDE_DIR}
)

target_link_libraries(nodescm PRIVATE scm_lib)
