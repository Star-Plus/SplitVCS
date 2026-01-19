cmake_minimum_required(VERSION 3.29)
project(SplitLog)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SplitLog STATIC
    src/main/Logger.cpp
    src/utils/Color.h
    src/utils/Color.cpp
    src/utils/Format.h

    include/SplitLog.h
)

target_include_directories(SplitLog PUBLIC src)

add_subdirectory(playground)