cmake_minimum_required(VERSION 3.29)
project(SplitAsset)

add_library(SplitAsset STATIC
        src/enums/AssetType.h
        src/enums/AssetType.cpp
        src/utils/AssetInfo.h
        src/utils/AssetInfo.cpp
        src/atoms/Asset.h
)

target_include_directories(SplitAsset PRIVATE src)