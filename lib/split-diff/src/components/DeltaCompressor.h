#pragma once
#include "components/EncoderFactory.h"
#include "components/DecoderFactory.h"
#include "atoms/Blob.h"

namespace Split {

    class DeltaCompressor {
    public: 

        DeltaCompressor();

        void encode(const Blob& v1, const Blob& v2, const Blob& out);

        void decode(const std::istream& base, const std::istream* deltas, std::ostream& out);

        void decode(const Blob& base, const Blob& delta, const Blob& out);

    private:
        EncoderFactory encoderFactory;
        DecoderFactory decoderFactory;
    };

}