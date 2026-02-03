#pragma once
#include "interfaces/IDecoder.h"
#include "enums/EncodeType.h"
#include "features/binary/ByteDecoder.h"
#include "features/images/ImageDecoder.h"
#include "features/photoshop/PsdDecoder.h"

namespace Split {

    class DecoderFactory {
    public:

        DecoderFactory() :
        imageDecoder(new ImageDecoder()),
        byteDecoder(new ByteDecoder()),
        psdDecoder(new PsdDecoder())
        {}

        Decoder* getDecoder(const EncodeType type) const {
            switch (type)
            {
            case EncodeType::IMAGE:
                return this->imageDecoder;
            case EncodeType::BINARY:
                return this->byteDecoder;
            case EncodeType::PSD:
                return this->psdDecoder;
            
                default:
                    throw std::invalid_argument("Unknown decoder type");
            }
        }

    private:
        ImageDecoder* imageDecoder;
        ByteDecoder* byteDecoder;
        PsdDecoder* psdDecoder;
    };

}
