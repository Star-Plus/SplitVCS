#pragma once
#include "interfaces/IEncoder.h"
#include "features/images/ImageEncoder.h"
#include "enums/EncodeType.h"
#include "features/binary/ByteEncoder.h"
#include "features/photoshop/PsdEncoder.h"

namespace Split {
    class PsdEncoder;

    class EncoderFactory {
    public:
        EncoderFactory() :
        imageEncoder(new ImageEncoder()),
        byteEncoder(new ByteEncoder()),
        psdEncoder(new PsdEncoder())
        {}

        Encoder* getEncoder(const EncodeType type) const {
            switch (type)
            {
                case EncodeType::IMAGE:
                    return this->imageEncoder;

                case EncodeType::BINARY:
                    return this->byteEncoder;

                case EncodeType::PSD:
                    return this->psdEncoder;
            
                default:
                    throw std::invalid_argument("Unknown encoder type");
            }
        }

    private:
        ImageEncoder* imageEncoder;
        ByteEncoder* byteEncoder;
        PsdEncoder* psdEncoder;
    };

}
