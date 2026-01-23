#include "AssetType.h"

#include <stdexcept>

#include "utils/AssetInfo.h"

namespace Split
{

    AssetType operator&(const AssetType lhs, const AssetType rhs)
    {
        if (lhs == rhs) return lhs;

        throw std::logic_error("not implemented");
    }

    namespace Assets
    {
        AssetType assetTypeFromPath(const std::string& path) {

            const std::string extension = path.substr(path.find_last_of('.'));

            if (AssetInfo::isImage(extension)) {
                return AssetType::IMAGE;
            }

            return AssetType::BINARY;
        }

        std::string typeToString(const AssetType& assetType)
        {
            switch (assetType)
            {
                case AssetType::IMAGE:
                    return "image";
                case AssetType::BINARY:
                    return "binary";
                default:
                    throw std::logic_error("not implemented");
            }
        }

        AssetType typeFromString(const std::string& assetType)
        {
            if (assetType == "image") return AssetType::IMAGE;
            if (assetType == "binary") return AssetType::BINARY;
            throw std::logic_error("not implemented");
        }
    }

}