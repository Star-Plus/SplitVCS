//
// Created by lightum on 1/19/26.
//

#ifndef SPLITVCS_IMAGECOMPRESSOR_H
#define SPLITVCS_IMAGECOMPRESSOR_H

#include "interfaces/ICompressor.h"

namespace Split {

    class ImageCompressor final : public ICompressor{

    public:
        void encode(const std::string& source, const std::string& save, EncodeOptions options = {}) override;

    };
}

#endif //SPLITVCS_IMAGECOMPRESSOR_H