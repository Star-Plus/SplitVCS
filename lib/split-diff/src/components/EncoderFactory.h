#pragma once
#include "interfaces/IEncoder.h"
#include "features/images/ImageEncoder.h"
#include "enums/AssetType.h"

namespace Split {

    class EncoderFactory {
    public:
        EncoderFactory() :
        imageEncoder(new ImageEncoder())
        {}

        Encoder* getEncoder(const AssetType type) const {
            switch (type)
            {
            case AssetType::IMAGE:
                return this->imageEncoder;
            
            default:
                throw std::invalid_argument("Unknown encoder type");
            }
        }

    private:
        ImageEncoder* imageEncoder;
    };

}