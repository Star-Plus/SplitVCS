cmake_minimum_required(VERSION 3.29)
project(SplitAsset)

add_library(SplitAsset STATIC
        src/enums/EncodeType.h
        src/enums/EncodeType.cpp
        src/utils/AssetInfo.h
        src/utils/AssetInfo.cpp
        src/atoms/Asset.h
        src/utils/path/PathUtils.cpp
        src/utils/path/PathUtils.h
)

target_include_directories(SplitAsset PUBLIC src)

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
add_executable(SplitAssetTests
        src/utils/path/tests/PathUtils.test.cpp
)
target_link_libraries(SplitAssetTests PRIVATE SplitAsset gtest_main)

# --- Discover tests automatically ---
include(GoogleTest)
gtest_discover_tests(SplitAssetTests)