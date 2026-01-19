#include "interfaces/IDecoder.h"

namespace Split {

    class ImageDecoder : public Decoder {
    public:
        void decode(const std::istream& base, const std::istream& delta, std::ostream& out) override;
    };

}