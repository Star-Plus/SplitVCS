#pragma once
#include "interfaces/IDecoder.h"
#include "enums/AssetType.h"
#include "features/binary/ByteDecoder.h"
#include "features/images/ImageDecoder.h"

namespace Split {

    class DecoderFactory {
    public:

        DecoderFactory() :
        imageDecoder(new ImageDecoder()),
        byteDecoder(new ByteDecoder())
        {}

        Decoder* getDecoder(const AssetType type) const {
            switch (type)
            {
                case AssetType::IMAGE:
                    return this->imageDecoder;
                case AssetType::BINARY:
                    return this->byteDecoder;
            
                default:
                    throw std::invalid_argument("Unknown decoder type");
            }
        }

    private:
        ImageDecoder* imageDecoder;
        ByteDecoder* byteDecoder;
    };

}
