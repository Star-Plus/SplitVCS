#include "DeltaCompressor.h"

namespace Split {

    DeltaCompressor::DeltaCompressor() = default;

    void DeltaCompressor::encode(const Blob& v1, const Blob& v2, const Blob& out) const {
        const auto encoder = encoderFactory.getEncoder(v2.type);
        encoder->encode(*v1.getInputStream(), *v2.getInputStream(), *out.getOutputStream());
    }

    void DeltaCompressor::decode(const Blob& base, std::stack<std::unique_ptr<Blob>>& deltas, const Blob& out) const {

        auto middleIn = new Blob(*base.getInputStream());
        middleIn->type = base.type;

        if (deltas.empty())
        {
            *out.getOutputStream() << base.getInputStream()->rdbuf();
            delete middleIn;
            return;
        }

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
                const Blob middleOut(outStream);

                this->decode(
                    *middleIn,
                    *delta,
                    middleOut
                );

                delete middleIn;

                middleIn = new Blob(outStream);
                middleIn->type = base.type;
            }

            deltas.pop();
        }
    }

    void DeltaCompressor::decode(const Blob& base, const Blob& delta, const Blob& out) const {
        const auto decoder = decoderFactory.getDecoder(delta.type);
        decoder->decode(*base.getInputStream(), *delta.getInputStream(), *out.getOutputStream());
    }

}