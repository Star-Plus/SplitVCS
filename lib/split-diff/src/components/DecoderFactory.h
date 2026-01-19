#include "interfaces/IDecoder.h"
#include "enums/AssetType.h"

namespace Split {

    class DecoderFactory {
    public:
        Decoder* getDecoder(const AssetType type) {
            switch (type)
            {
            case AssetType::IMAGE:
                break;
            
            default:
                throw std::invalid_argument("Unknown decoder type");
            }

            return nullptr;
        }
    };

}