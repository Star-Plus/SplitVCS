//
// Created by lightum on 1/19/26.
//

#ifndef SPLITVCS_COMPRESSORFACTORY_H
#define SPLITVCS_COMPRESSORFACTORY_H

#include "enums/AssetType.h"
#include "features/ImageCompressor.h"

namespace Split {

    class CompressorFactory {

    public:

        CompressorFactory():
            imageCompressor(new ImageCompressor())
        {}

        ICompressor* getCompressor(const AssetType type) const {
            switch (type) {
                case AssetType::IMAGE:
                    return this->imageCompressor;
                default:
                    return nullptr;
            }
        }

    private:
        ImageCompressor* imageCompressor;

    };
}
#endif //SPLITVCS_COMPRESSORFACTORY_H