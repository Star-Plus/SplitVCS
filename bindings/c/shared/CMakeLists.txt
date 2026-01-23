cmake_minimum_required(VERSION 3.29)
project(split_c_api)

add_library(split_c_api SHARED
        split_c_api.cpp
)

target_compile_definitions(split_c_api PRIVATE
        SPLIT_C_EXPORTS
)

target_link_libraries(split_c_api PRIVATE
        scm_lib
)

target_include_directories(split_c_api PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)