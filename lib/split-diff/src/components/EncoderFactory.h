#pragma once
#include "interfaces/IEncoder.h"
#include "features/images/ImageEncoder.h"
#include "enums/AssetType.h"
#include "features/binary/ByteEncoder.h"

namespace Split {

    class EncoderFactory {
    public:
        EncoderFactory() :
        imageEncoder(new ImageEncoder()),
        byteEncoder(new ByteEncoder())
        {}

        Encoder* getEncoder(const AssetType type) const {
            switch (type)
            {
                case AssetType::IMAGE:
                    return this->imageEncoder;

                case AssetType::BINARY:
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
