#include "enums/AssetType.h"
#include "utils/AssetInfo.h"

namespace Split {

    AssetType assetTypeFromPath(const std::string& path) {

        const std::string extension = path.substr(path.find_last_of('.'));

        if (AssetInfo::isImage(extension)) {
            return AssetType::IMAGE;
        }

        return AssetType::BINARY;
    }
}