#include "DeltaCompressor.h"

namespace Split {

    DeltaCompressor::DeltaCompressor() {}

    void DeltaCompressor::encode(const Blob& v1, const Blob& v2, const Blob& out) {
        const AssetType type = assetTypeFromPath(v1.path);

        const auto encoder = encoderFactory.getEncoder(type);
        encoder->encode(*v1.getInputStream(), *v2.getInputStream(), *out.getOutputStream());
    }

    void DeltaCompressor::decode(const std::istream& base, const std::istream* deltas, std::ostream& out) {

    }

    void DeltaCompressor::decode(const Blob& base, const Blob& delta, const Blob& out) {
        const AssetType type = assetTypeFromPath(base.path);
        const auto decoder = decoderFactory.getDecoder(type);
        decoder->decode(*base.getInputStream(), *delta.getInputStream(), *out.getOutputStream());
    }

}