cmake_minimum_required(VERSION 3.14)
project(nodescm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

xinclude(FetchContent)
FetchContent_Declare(
        node_addon_api
        GIT_REPOSITORY https://github.com/nodejs/node-addon-api.git
        GIT_TAG main
)
FetchContent_MakeAvailable(node_addon_api)

set(NODE_INCLUDE_DIR "C:/Program Files/nodejs/include/node")

add_library(nodescm SHARED bindings.cpp)
set_target_properties(nodescm PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(nodescm
        PRIVATE
        ${node_addon_api_SOURCE_DIR}
        ${NODE_INCLUDE_DIR}
)

target_link_libraries(nodescm PRIVATE scm_lib)
