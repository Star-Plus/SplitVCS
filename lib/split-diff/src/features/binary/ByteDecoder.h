//
// Created by lightum on 1/20/26.
//

#ifndef SPLITVCS_BYTEDECODER_H
#define SPLITVCS_BYTEDECODER_H
#include "interfaces/IDecoder.h"

namespace Split {

    class ByteDecoder final : public Decoder {
    public:
        ByteDecoder() {}

        void decode(const std::istream &base, const std::istream &delta, std::ostream &out) override;
    };

}

#endif //SPLITVCS_BYTEDECODER_H