#pragma once
#include "interfaces/IDecoder.h"
#include "enums/AssetType.h"
#include "features/images/ImageDecoder.h"

namespace Split {

    class DecoderFactory {
    public:

        DecoderFactory() :
        imageDecoder(new ImageDecoder())
        {}

        Decoder* getDecoder(const AssetType type) const {
            switch (type)
            {
            case AssetType::IMAGE:
                return this->imageDecoder;
            
            default:
                throw std::invalid_argument("Unknown decoder type");
            }
        }

    private:
        ImageDecoder* imageDecoder;
    };

}
