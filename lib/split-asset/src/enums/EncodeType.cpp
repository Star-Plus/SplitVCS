#include "EncodeType.h"

#include <stdexcept>

#include "utils/AssetInfo.h"

namespace Split
{

    EncodeType operator&(const EncodeType lhs, const EncodeType rhs)
    {
        if (lhs == rhs) return lhs;

        throw std::logic_error("not implemented");
    }

    namespace Assets
    {
        EncodeType encodeTypeFromPath(const std::string& path) {

            const std::string extension = path.substr(path.find_last_of('.'));

            if (AssetInfo::isImage(extension)) {
                return EncodeType::IMAGE;
            }

            if (extension == ".psd") return EncodeType::PSD;

            return EncodeType::BINARY;
        }

        std::string typeToString(const EncodeType& assetType)
        {
            switch (assetType)
            {
            case EncodeType::IMAGE:
                return "image";
            case EncodeType::BINARY:
                return "binary";
            case EncodeType::PSD:
                return "psd";
            case EncodeType::EXACT:
                return "exact";
                default:
                    throw std::logic_error("not implemented");
            }
        }

        EncodeType typeFromString(const std::string& encodeType)
        {
            if (encodeType == "image") return EncodeType::IMAGE;
            if (encodeType == "binary") return EncodeType::BINARY;
            if (encodeType == "exact") return EncodeType::EXACT;
            if (encodeType == "psd") return EncodeType::PSD;
            throw std::logic_error("not implemented");
        }
    }

}