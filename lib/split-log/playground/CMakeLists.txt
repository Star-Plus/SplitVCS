cmake_minimum_required(VERSION 3.29)
project(SplitLogPlayground)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SplitLogPlayground main.cpp)
target_link_libraries(SplitLogPlayground PRIVATE SplitLog)