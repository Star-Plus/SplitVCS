cmake_minimum_required(VERSION 3.29)
project(SplitLive)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SplitLive STATIC
        core/LiveEncoder.cpp
        core/LiveEncoder.h
)

target_include_directories(SplitLive PUBLIC
        ${CMAKE_BINARY_DIR}
)

add_subdirectory(playground)