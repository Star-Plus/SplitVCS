#pragma once
#include "interfaces/IDecoder.h"
#include "main/Logger.h"

namespace Split {

    class ImageDecoder final : public Decoder {
    public:
        void decode(const std::istream& base, const std::istream& delta, std::ostream& out) override;
    private:
        Logger logger;
    };

}