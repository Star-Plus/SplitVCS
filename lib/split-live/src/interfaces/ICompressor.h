//
// Created by lightum on 1/19/26.
//

#ifndef SPLITVCS_ICOMPRESSOR_H
#define SPLITVCS_ICOMPRESSOR_H

#include <string>
#include <enums/AssetType.h>

namespace Split {

    struct EncodeOptions {
        int quality = 50;
        AssetType type = AssetType::BINARY;
    };

    class ICompressor {
    public:
        virtual ~ICompressor() = default;

        virtual std::string encode(const std::string& src, const std::string& save, EncodeOptions options = {}) = 0;
    };

}

#endif //SPLITVCS_ICOMPRESSOR_H