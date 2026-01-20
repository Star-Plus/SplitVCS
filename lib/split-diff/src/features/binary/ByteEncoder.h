//
// Created by lightum on 1/20/26.
//

#ifndef SPLITVCS_BYTEENCODER_H
#define SPLITVCS_BYTEENCODER_H
#include "interfaces/IEncoder.h"

namespace Split {

    class ByteEncoder final : public Encoder {
    public:
        ByteEncoder() = default;
        ~ByteEncoder() override = default;

        void encode(const std::istream& v1, const std::istream& v2, std::ostream& out) override;

    };

}


#endif //SPLITVCS_BYTEENCODER_H