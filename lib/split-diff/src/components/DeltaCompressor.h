#pragma once
#include "components/EncoderFactory.h"
#include "components/DecoderFactory.h"
#include "atoms/Blob.h"
#include <stack>

namespace Split {

    class DeltaCompressor {
    public: 

        DeltaCompressor();

        void encode(const Blob& v1, const Blob& v2, const Blob& out) const;

        void decode(const Blob& base, std::stack<std::unique_ptr<Blob>>& deltas, const Blob& out) const;

        void decode(const Blob& base, const Blob& delta, const Blob& out) const;

    private:
        EncoderFactory encoderFactory;
        DecoderFactory decoderFactory;
    };

}