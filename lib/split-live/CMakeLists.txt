cmake_minimum_required(VERSION 3.29)
project(SplitLive)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_library(SplitLive STATIC
        src/LiveEncoder.cpp
        src/LiveEncoder.h
        src/features/ImageCompressor.cpp
        src/features/ImageCompressor.h
        src/components/CompressorFactory.h
        src/interfaces/ICompressor.h
        src/features/TextCompressor.cpp
        src/features/TextCompressor.h
        src/features/photoshop/PsdUltraviolet.cpp
        src/features/photoshop/PsdUltraviolet.h
)

target_link_libraries(SplitLive PRIVATE SplitAsset
        opencv_core opencv_imgproc opencv_imgcodecs
        Psd
)

target_include_directories(SplitLive PUBLIC
        src
        ${SplitAsset_SOURCE_DIR}/src
        ${opencv_SOURCE_DIR}/include
        ${opencv_SOURCE_DIR}/modules/core/include
        ${opencv_SOURCE_DIR}/modules/imgproc/include
        ${opencv_SOURCE_DIR}/modules/imgcodecs/include
        ${PSDSDK_DIR}
        ${CMAKE_BINARY_DIR}
)

add_subdirectory(playground)

# --- Enable testing ---
enable_testing()
include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# --- Test executable ---
add_executable(SplitLiveTests
        src/features/photoshop/tests/PsdUltraviolet.test.cpp
)
target_link_libraries(SplitLiveTests PRIVATE SplitLive gtest_main)

# --- Discover tests automatically ---
include(GoogleTest)
gtest_discover_tests(SplitLiveTests)