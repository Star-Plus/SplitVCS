#include "DeltaCompressor.h"

namespace Split {

    DeltaCompressor::DeltaCompressor() = default;

    void DeltaCompressor::encode(const Blob& v1, const Blob& v2, const Blob& out) const {
        const AssetType type = assetTypeFromPath(v1.path);

        const auto encoder = encoderFactory.getEncoder(type);
        encoder->encode(*v1.getInputStream(), *v2.getInputStream(), *out.getOutputStream());
    }

    void DeltaCompressor::decode(const Blob& base, std::stack<std::unique_ptr<Blob>>& deltas, const Blob& out) const {

        auto middleIn = new Blob(base.getInputStream());
        middleIn->path = base.path;

        while (!deltas.empty()) {
            const auto delta = deltas.top().get();

            if (deltas.size() == 1) {
                this->decode(
                    *middleIn,
                    *delta,
                    out
                );
            } else {
                std::ostringstream outStream;
                const Blob middleOut(&outStream);

                this->decode(
                    *middleIn,
                    *delta,
                    middleOut
                );

                delete middleIn;

                middleIn = new Blob(&outStream);
                middleIn->path = base.path;
            }

            deltas.pop();
        }
    }

    void DeltaCompressor::decode(const Blob& base, const Blob& delta, const Blob& out) const {
        const AssetType type = assetTypeFromPath(base.path);
        const auto decoder = decoderFactory.getDecoder(type);
        decoder->decode(*base.getInputStream(), *delta.getInputStream(), *out.getOutputStream());
    }

}