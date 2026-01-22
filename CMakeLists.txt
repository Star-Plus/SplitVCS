cmake_minimum_required(VERSION 3.29)
project(SplitVCS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

# --- OpenVCDIFF Paths ---
set(OPENVCDIFF_INCLUDE "${CMAKE_SOURCE_DIR}/external/open-vcdiff/include")
set(OPENVCDIFF_LIB "${CMAKE_SOURCE_DIR}/external/open-vcdiff/lib")


# --- Main library for your project ---
add_library(scm_lib STATIC
        src/stores/ObjectStore.cpp
        src/stores/Index.cpp
        src/components/Repository.cpp
        src/utils/SHA1.h
        src/utils/Hashing.h
        src/utils/Time.h
        src/components/Commit.h
        src/components/Commit.cpp
        src/components/Tree.h
        src/components/Tree.cpp
        src/stores/Pack.cpp
        src/stores/Pack.h
        src/core/Alias.h
        src/SplitInterface.cpp
        src/SplitInterface.h
        src/features/RepositoryStatus.cpp
        src/features/RepositoryStatus.h
        src/features/PackNegotiation/PackNegotiation.cpp
        src/features/PackNegotiation/PackNegotiation.h
        src/features/PackNegotiation/CommitDiffer.cpp
        src/features/PackNegotiation/CommitDiffer.h
        src/features/PackNegotiation/DIFF_TYPE.h
        src/utils/Parsers/ParserToString.cpp
        src/utils/Parsers/ParserToString.h

        src/features/ManualCommit/ManualCommiter.h
        src/features/ManualCommit/ManualCommiter.cpp
        src/utils/Fs/PackOptimizer.cpp
        src/utils/Fs/PackOptimizer.h
        src/utils/Fs/FsMetadata.cpp
        src/utils/Fs/FsMetadata.h
        src/utils/Fs/DualStream.h
        src/features/IgnoreList/IgnoreList.cpp
        src/features/IgnoreList/IgnoreList.h
        src/utils/String/StringUtils.h
        src/features/FastLiveAsset/TinyAsset.cpp
        src/features/FastLiveAsset/TinyAsset.h
        src/features/FastLiveAsset/TinyAsset.cpp
        src/features/FastLiveAsset/TinyAsset.h
)

target_include_directories(scm_lib PUBLIC src lib)


# Link SplitDiff to scm_lib
add_subdirectory(lib/split-diff)
target_link_libraries(scm_lib PRIVATE SplitDiff)
target_include_directories(scm_lib PUBLIC lib/split-diff/src)

# --- Main executable ---
add_executable(SplitVCS main.cpp)
target_link_libraries(SplitVCS PRIVATE scm_lib)

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
add_executable(runTests
        tests/PackTest.cpp
        tests/ObjectStoreTest.cpp
        tests/IndexTest.cpp
        tests/TimeTest.cpp
        tests/TreeTest.cpp
        tests/RepositoryStatusTest.cpp
        tests/CommitDiffer.Test.cpp
        tests/PackNegotiation.Test.cpp
        tests/Checkout.Test.cpp
        src/utils/Fs/tests/FsMetadata.test.cpp
        tests/HandlingImage.Test.cpp
        src/features/IgnoreList/tests/IgnoreList.Test.cpp
)
target_link_libraries(runTests PRIVATE scm_lib gtest_main)

# --- Discover tests automatically ---
include(GoogleTest)
gtest_discover_tests(runTests)

# Bindings
add_subdirectory(python)
add_subdirectory(rust)

add_subdirectory(lib/split-log)
