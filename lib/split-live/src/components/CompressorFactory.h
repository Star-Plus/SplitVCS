//
// Created by lightum on 1/19/26.
//

#ifndef SPLITVCS_COMPRESSORFACTORY_H
#define SPLITVCS_COMPRESSORFACTORY_H

#include "enums/AssetType.h"
#include "features/ImageCompressor.h"
#include "features/TextCompressor.h"

namespace Split {

    class CompressorFactory {

    public:

        CompressorFactory():
            imageCompressor(new ImageCompressor()),
            textCompressor(new TextCompressor())
        {}

        ICompressor* getCompressor(const AssetType type) const {
            switch (type) {
            case AssetType::IMAGE:
                return this->imageCompressor;
            case AssetType::BINARY:
                return this->textCompressor;

            default:
                return nullptr;
            }
        }

    private:
        ImageCompressor* imageCompressor;
        TextCompressor* textCompressor;
    };
}
#endif //SPLITVCS_COMPRESSORFACTORY_H