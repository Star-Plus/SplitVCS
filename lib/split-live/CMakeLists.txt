project(SplitLive)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

include(FetchContent)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp
)

FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
        draco
        GIT_REPOSITORY https://github.com/google/draco
        GIT_TAG 1.5.7
)

set(DRACO_MESH_COMPRESSION ON CACHE BOOL "" FORCE)
set(DRACO_POINT_CLOUD_COMPRESSION ON CACHE BOOL "" FORCE)
set(DRACO_STANDARD_EDGEBREAKER ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(draco)


add_executable(SplitLive main.cpp
        3D/SceneLoader.cpp
        3D/SceneLoader.h
        3D/MeshAdapter.cpp
        3D/MeshAdapter.h)

target_link_libraries(SplitLive PRIVATE assimp draco::draco)
target_include_directories(SplitLive PRIVATE
        ${draco_SOURCE_DIR}/src
        ${draco_BINARY_DIR}
        ${assimp_SOURCE_DIR}/include
)

