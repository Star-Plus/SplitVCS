cmake_minimum_required(VERSION 3.29)
project(SplitLivePlayground)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SplitLivePlayground
        main.cpp
)

target_link_libraries(SplitLivePlayground PRIVATE SplitLive)