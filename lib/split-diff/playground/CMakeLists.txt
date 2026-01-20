cmake_minimum_required(VERSION 3.29)
project(SplitDiffPlayground)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SplitDiffPlayground main.cpp)
target_link_libraries(SplitDiffPlayground PRIVATE SplitDiff)