#pragma once
#include <string>

namespace Split {

    enum class AssetType {
        IMAGE,
        BINARY
    };

    AssetType assetTypeFromPath(const std::string& path);

}