#include "DeltaCompressor.h"

#include <iostream>

namespace Split {

    DeltaCompressor::DeltaCompressor() : logger(true) {}

    void DeltaCompressor::encode(const Blob& v1, const Blob& v2, const Blob& out) const {
        const auto encoder = encoderFactory.getEncoder(v2.type);
        encoder->encode(*v1.getInputStream(), *v2.getInputStream(), *out.getOutputStream());
    }

    std::string DeltaCompressor::encode(const Asset& base, const std::vector<Asset>& deltas, const Asset& v2, Asset& out) const
    {
        const auto encoder = encoderFactory.getEncoder(v2.type);

        std::stack<std::string> deltaPaths;
        for (int i = deltas.size() - 1; i >= 0; --i)
        {
            deltaPaths.push(deltas[i].path);
        }

        return encoder->encode(base.path, deltaPaths, v2.path, out.path);
    }

    void DeltaCompressor::decode(const Blob& base, std::stack<std::unique_ptr<Blob>>& deltas, const Blob& out) const
    {

        if (deltas.empty())
        {
            *out.getOutputStream() << base.getInputStream()->rdbuf();
            return;
        }

        std::stringstream current;
        current << base.getInputStream()->rdbuf();
        current.seekg(0);

        while (!deltas.empty()) {
            const auto delta = *deltas.top();
            deltas.pop();

            if (deltas.empty()) {
                singleDecode(current, *delta.getInputStream(), *out.getOutputStream(), delta.type);
            } else {
                std::stringstream next;

                singleDecode(current, *delta.getInputStream(), next, delta.type);
                current.swap(next);
                current.seekg(0);
            }
        }
    }

    void DeltaCompressor::decode(const Asset& base, const std::vector<Asset>& deltas, Asset& out) const
    {
        const auto decoder = decoderFactory.getDecoder(base.type);

        std::stack<std::string> deltaPaths;
        for (int i = deltas.size() - 1; i >= 0; --i)
        {
            deltaPaths.push(deltas[i].path);
        }

        decoder->decode(base.path, deltaPaths, out.path);
    }

    void DeltaCompressor::singleDecode(const std::istream& base, const std::istream& delta, std::ostream& out, const AssetType encodeType) const {
        const auto decoder = decoderFactory.getDecoder(encodeType);
        decoder->decode(base, delta, out);
    }

}
