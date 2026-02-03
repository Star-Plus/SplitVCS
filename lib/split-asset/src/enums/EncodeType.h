#pragma once
#include <string>

namespace Split {

    enum class EncodeType {
        IMAGE,
        BINARY,
        PSD,
        EXACT
    };

    EncodeType operator&(EncodeType lhs, EncodeType rhs);

    namespace Assets
    {
        EncodeType encodeTypeFromPath(const std::string& path);
        std::string typeToString(const EncodeType& assetType);
        EncodeType typeFromString(const std::string& encodeType);
    }

}