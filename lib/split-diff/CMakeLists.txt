cmake_minimum_required(VERSION 3.29)
project(SplitDiff LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(BUILD_LIST core,imgproc,imgcodecs CACHE STRING "" FORCE)
set(BUILD_opencv_world OFF CACHE BOOL "" FORCE)
set(WITH_IPP OFF CACHE BOOL "" FORCE)
set(WITH_TBB OFF CACHE BOOL "" FORCE)
set(WITH_OPENMP OFF CACHE BOOL "" FORCE)
set(WITH_CUDA OFF CACHE BOOL "" FORCE)
set(WITH_MSMF OFF CACHE BOOL "" FORCE)
set(WITH_QT OFF CACHE BOOL "" FORCE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

include(FetchContent)


FetchContent_Declare(
        opencv_contrib
        GIT_REPOSITORY https://github.com/opencv/opencv_contrib
        GIT_TAG 4.x
)
FetchContent_MakeAvailable(opencv_contrib)

FetchContent_GetProperties(opencv_contrib)
if (NOT opencv_contrib_POPULATED)
    FetchContent_Populate(opencv_contrib)
endif ()

set(OPENCV_EXTRA_MODULES_PATH ${opencv_contrib_SOURCE_DIR}/modules CACHE PATH "" FORCE)

FetchContent_Declare(
        opencv
        GIT_REPOSITORY https://github.com/opencv/opencv
        GIT_TAG 4.x
)
FetchContent_MakeAvailable(opencv)



FetchContent_Declare(
        zstd
        GIT_REPOSITORY https://github.com/facebook/zstd
        GIT_TAG v1.5.5
        SOURCE_SUBDIR build/cmake
)

FetchContent_MakeAvailable(zstd)

add_executable(SplitDiff main.cpp
        core/Encoder.h)

target_link_libraries(SplitDiff PRIVATE opencv_core opencv_imgproc opencv_imgcodecs)
target_include_directories(SplitDiff PRIVATE
        ${opencv_SOURCE_DIR}/include
        ${opencv_SOURCE_DIR}/modules/core/include
        ${opencv_SOURCE_DIR}/modules/imgproc/include
        ${opencv_SOURCE_DIR}/modules/imgcodecs/include
        ${CMAKE_BINARY_DIR}
)

target_link_libraries(SplitDiff PRIVATE libzstd_static)
target_include_directories(SplitDiff PRIVATE
        ${zstd_SOURCE_DIR}/lib
)