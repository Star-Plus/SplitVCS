//
// Created by lightum on 1/19/26.
//

#ifndef SPLITVCS_COMPRESSORFACTORY_H
#define SPLITVCS_COMPRESSORFACTORY_H

#include "enums/EncodeType.h"
#include "features/ImageCompressor.h"
#include "features/TextCompressor.h"
#include "features/photoshop/PsdUltraviolet.h"

namespace Split {

    class CompressorFactory {

    public:

        CompressorFactory():
            imageCompressor(new ImageCompressor()),
            textCompressor(new TextCompressor()),
            psdUltraviolet(new PsdUltraviolet())
        {}

        ICompressor* getCompressor(const EncodeType type) const {
            switch (type) {
            case EncodeType::IMAGE:
                return this->imageCompressor;
            case EncodeType::BINARY:
                return this->textCompressor;
            case EncodeType::PSD:
                return this->psdUltraviolet;

            default:
                return nullptr;
            }
        }

    private:
        ImageCompressor* imageCompressor;
        TextCompressor* textCompressor;
        PsdUltraviolet* psdUltraviolet;
    };
}
#endif //SPLITVCS_COMPRESSORFACTORY_H