#pragma once
#include "components/EncoderFactory.h"
#include "components/DecoderFactory.h"
#include "atoms/Blob.h"
#include <stack>
#include <main/Logger.h>

#include "atoms/Asset.h"

namespace Split {

    class DeltaCompressor {
    public: 

        DeltaCompressor();

        std::string encode(const Asset& base, const Asset& out) const;
        std::string encode(const Asset& base, const std::vector<Asset>& deltas, const Asset& v2, Asset& out) const;

        void decode(const Asset& base, const std::vector<Asset>& deltas, Asset& out) const;

        void singleDecode(const std::istream& base, const std::istream& delta, std::ostream& out, EncodeType encodeType) const;

    private:
        EncoderFactory encoderFactory;
        DecoderFactory decoderFactory;

        void encode(const Blob& v1, const Blob& v2, const Blob& out) const;
        void decode(const Blob& base, std::stack<std::unique_ptr<Blob>>& deltas, const Blob& out) const;

        Logger logger;
    };

}
