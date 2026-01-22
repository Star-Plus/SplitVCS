cmake_minimum_required(VERSION 3.29)
project(SplitDiffAnalysis)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SplitDiffAnalysis
        ImageScan.cpp
        BackwardDelta.cpp
)

target_link_libraries(SplitDiffAnalysis PUBLIC SplitDiff)
target_include_directories(SplitDiffAnalysis PUBLIC
        ${SplitDiff_SOURCE_DIR}/src
        ${opencv_SOURCE_DIR}/include
        ${opencv_SOURCE_DIR}/modules/core/include
        ${opencv_SOURCE_DIR}/modules/imgproc/include
        ${opencv_SOURCE_DIR}/modules/imgcodecs/include
        ${CMAKE_BINARY_DIR}
)