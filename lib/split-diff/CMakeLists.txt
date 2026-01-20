cmake_minimum_required(VERSION 3.29)
project(SplitDiff)

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

#FetchContent_Declare(
#        zstd
#        GIT_REPOSITORY https://github.com/facebook/zstd
#        GIT_TAG v1.5.5
#        SOURCE_SUBDIR build/cmake
#)
#
#FetchContent_MakeAvailable(zstd)

add_library(SplitDiff STATIC
        src/components/DeltaCompressor.cpp
        src/interfaces/IDecoder.h
        src/interfaces/IEncoder.h
        src/components/EncoderFactory.h
        src/components/DecoderFactory.h
        src/features/images/ImageDecoder.h
        src/features/images/ImageDecoder.cpp
        src/features/images/ImageEncoder.h
        src/features/images/ImageEncoder.cpp
        src/enums/AssetType.h
        src/enums/AssetType.cpp
        src/utils/AssetInfo.h
        src/utils/AssetInfo.cpp
        src/atoms/Blob.h
        src/features/images/debug/ImageDebugUtils.cpp
        src/features/images/utils/ReadAdapter.h
        src/features/images/utils/FormatSniffer.h
        src/features/binary/ByteEncoder.cpp
        src/features/binary/ByteEncoder.h
        src/features/binary/ByteDecoder.cpp
        src/features/binary/ByteDecoder.h
)


# --- OpenVCDIFF Paths ---
set(OPENVCDIFF_INCLUDE "${CMAKE_SOURCE_DIR}/external/open-vcdiff/include")
set(OPENVCDIFF_LIB "${CMAKE_SOURCE_DIR}/external/open-vcdiff/lib")

if (WIN32)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPENVCDIFF_LIB "${OPENVCDIFF_LIB}/Windows/Debug")
    else()
        set(OPENVCDIFF_LIB "${OPENVCDIFF_LIB}/Windows/Release")
    endif()

    file(GLOB OPENVCDIFF_LIBS "${OPENVCDIFF_LIB}/*.lib")
    target_link_libraries(SplitDiff PRIVATE ${OPENVCDIFF_LIBS})
elseif(UNIX)
    target_link_libraries(SplitDiff PRIVATE
            ${OPENVCDIFF_LIB}/libvcddec.a
            ${OPENVCDIFF_LIB}/libvcdenc.a
            ${OPENVCDIFF_LIB}/libvcdcom.a
    )
else()
    message(FATAL_ERROR "Unsupported platform. Please add support for your platform.")
endif()

target_link_libraries(SplitDiff PRIVATE SplitLog opencv_core opencv_imgproc opencv_imgcodecs)
target_include_directories(SplitDiff PUBLIC
        src
        ${SplitLog_SOURCE_DIR}/src
        ${opencv_SOURCE_DIR}/include
        ${opencv_SOURCE_DIR}/modules/core/include
        ${opencv_SOURCE_DIR}/modules/imgproc/include
        ${opencv_SOURCE_DIR}/modules/imgcodecs/include
        ${OPENVCDIFF_INCLUDE}
        ${CMAKE_BINARY_DIR}
#        ${zstd_SOURCE_DIR}/lib
)

add_subdirectory(playground)
