#pragma once
#include <string>

namespace Split {

    enum class AssetType {
        IMAGE,
        BINARY
    };

    AssetType operator&(AssetType lhs, AssetType rhs);

    namespace Assets
    {
        AssetType assetTypeFromPath(const std::string& path);
        std::string typeToString(const AssetType& assetType);
        AssetType typeFromString(const std::string& assetType);
    }

}