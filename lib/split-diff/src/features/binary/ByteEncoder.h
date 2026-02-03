//
// Created by lightum on 1/20/26.
//

#ifndef SPLITVCS_BYTEENCODER_H
#define SPLITVCS_BYTEENCODER_H
#include "interfaces/IEncoder.h"

#include <set>

#include "utils/stream/OffsetBound.h"

namespace Split {

    class ByteEncoder final : public Encoder {
    public:
        ByteEncoder() = default;
        ~ByteEncoder() override = default;

        std::string encode(const std::string& base, const std::string& out) override;
        std::string encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out) override;

    };

}


#endif //SPLITVCS_BYTEENCODER_H