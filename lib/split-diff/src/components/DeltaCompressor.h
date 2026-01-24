#pragma once
#include "components/EncoderFactory.h"
#include "components/DecoderFactory.h"
#include "atoms/Blob.h"
#include <stack>
#include <main/Logger.h>

namespace Split {

    class DeltaCompressor {
    public: 

        DeltaCompressor();

        void encode(const Blob& v1, const Blob& v2, const Blob& out) const;

        void decode(const Blob& base, std::stack<std::unique_ptr<Blob>>& deltas, const Blob& out) const;

        void singleDecode(const std::istream& base, const std::istream& delta, std::ostream& out, AssetType encodeType) const;

    private:
        EncoderFactory encoderFactory;
        DecoderFactory decoderFactory;

        Logger logger;
    };

}