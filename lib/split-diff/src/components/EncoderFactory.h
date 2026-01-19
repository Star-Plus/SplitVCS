#include "interfaces/IEncoder.h"
#include "enums/AssetType.h"

namespace Split {

    class EncoderFactory {
    public:
        Encoder* getEncoder(const AssetType type) {
            switch (type)
            {
            case AssetType::IMAGE:
                break;
            
            default:
                throw std::invalid_argument("Unknown encoder type");
            }

            return nullptr;
        }
    };

}