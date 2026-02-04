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

FetchContent_Declare(
        bit7z
        GIT_REPOSITORY https://github.com/rikyoz/bit7z
)
FetchContent_MakeAvailable(bit7z)

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
        src/atoms/Blob.h
        src/features/images/debug/ImageDebugUtils.cpp
        src/features/images/utils/ReadAdapter.h
        src/features/images/utils/FormatSniffer.h
        src/features/binary/ByteEncoder.cpp
        src/features/binary/ByteEncoder.h
        src/features/binary/ByteDecoder.cpp
        src/features/binary/ByteDecoder.h
        src/features/images/utils/PixelCalculator.h
        src/features/images/utils/PixelCalculator.cpp
        src/utils/psd/ChannelUtils.h
        src/utils/psd/TrackingFile.h
        src/features/photoshop/PsdEncoder.cpp
        src/features/photoshop/PsdEncoder.h
        src/features/photoshop/utils/PsdMatAdapter.cpp
        src/features/photoshop/utils/PsdMatAdapter.h
        src/features/photoshop/PsdDecoder.cpp
        src/features/photoshop/PsdDecoder.h
        src/features/photoshop/PsdDecoder.cpp
        src/features/photoshop/PsdDecoder.h
        src/utils/stream/OffsetBound.h
        src/utils/stream/OffsetBound.cpp
        src/features/dissolve/AssetDissolver.cpp
        src/features/dissolve/AssetDissolver.h
        src/features/dissolve/AssetDissolver.cpp
        src/features/dissolve/AssetDissolver.h
        src/features/dissolve/AssetBlockBuilder.cpp
        src/features/dissolve/AssetBlockBuilder.h
        src/features/dissolve/BlockUnit.h
        src/utils/adapters/OpenCvAdapter.h
        src/features/photoshop/utils/PsdLayersMetadata.h
        src/features/photoshop/utils/PsdLayerMetadataParser.h
        src/utils/psd/PsdChannelSizeUpdater.h
        src/utils/psd/PsdChannelSizeUpdater.cpp
        src/utils/hashing/RandomHash.cpp
        src/utils/hashing/RandomHash.h
        src/utils/hashing/RandomHash.cpp
        src/utils/hashing/RandomHash.h
        src/utils/compress/Bit7Archive.cpp
        src/utils/compress/Bit7Archive.h
        src/features/photoshop/utils/DecodeUtils.h
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
elseif(UNIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPENVCDIFF_LIB_DIR "${OPENVCDIFF_LIB}/Linux/Debug")
    else()
        set(OPENVCDIFF_LIB_DIR "${OPENVCDIFF_LIB}/Linux/Release")
    endif()

    set(OPENVCDIFF_LIBS
            "${OPENVCDIFF_LIB_DIR}/libvcdenc.a"
            "${OPENVCDIFF_LIB_DIR}/libvcddec.a"
            "${OPENVCDIFF_LIB_DIR}/libvcdcom.a"
    )
else()
    message(FATAL_ERROR "Unsupported platform. Please add support for your platform.")
endif()

target_link_libraries(SplitDiff PUBLIC
        SplitLog SplitAsset
        opencv_core opencv_imgproc opencv_imgcodecs
        ${OPENVCDIFF_LIBS}
        Psd
        bit7z
)

target_include_directories(SplitDiff PUBLIC
        src
        ${SplitLog_SOURCE_DIR}/src
        ${SplitAsset_SOURCE_DIR}/src
        ${opencv_SOURCE_DIR}/include
        ${PSDSDK_DIR}
        ${opencv_SOURCE_DIR}/modules/core/include
        ${opencv_SOURCE_DIR}/modules/imgproc/include
        ${opencv_SOURCE_DIR}/modules/imgcodecs/include
        ${OPENVCDIFF_INCLUDE}

        ${bit7z_SOURCE_DIR}/include

        ${CMAKE_BINARY_DIR}
)

add_subdirectory(playground)
add_subdirectory(analysis)

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
add_executable(SplitDiffTests
        tests/PsdIntegration.test.cpp
        src/features/photoshop/tests/PsdEncoder.test.cpp
        src/features/photoshop/tests/PsdDecoder.test.cpp
        src/utils/compress/tests/Bit7ZArchive.test.cpp
)
target_link_libraries(SplitDiffTests PRIVATE SplitDiff gtest_main)

# --- Discover tests automatically ---
include(GoogleTest)
gtest_discover_tests(SplitDiffTests)

add_custom_command(TARGET SplitDiffTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/dll/7z.dll"
        $<TARGET_FILE_DIR:SplitDiffTests>
)