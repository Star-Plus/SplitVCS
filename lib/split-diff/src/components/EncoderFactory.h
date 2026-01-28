#pragma once
#include "interfaces/IEncoder.h"
#include "features/images/ImageEncoder.h"
#include "enums/EncodeType.h"
#include "features/binary/ByteEncoder.h"

namespace Split {

    class EncoderFactory {
    public:
        EncoderFactory() :
        imageEncoder(new ImageEncoder()),
        byteEncoder(new ByteEncoder())
        {}

        Encoder* getEncoder(const EncodeType type) const {
            switch (type)
            {
                case EncodeType::IMAGE:
                    return this->imageEncoder;

                case EncodeType::BINARY:
                    return this->byteEncoder;
            
                default:
                    throw std::invalid_argument("Unknown encoder type");
            }
        }

    private:
        ImageEncoder* imageEncoder;
        ByteEncoder* byteEncoder;
    };

}
