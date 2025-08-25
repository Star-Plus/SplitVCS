cmake_minimum_required(VERSION 3.29)
project(SCM_RustBindings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_library(scm_bindings STATIC
        Interface.cpp
)

target_link_libraries(scm_bindings PRIVATE scm_lib)

target_include_directories(scm_bindings PUBLIC
        ${SCM_LIB_PATH}/src
)
