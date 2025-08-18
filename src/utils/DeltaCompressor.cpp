#include "DeltaCompressor.h"

#include <stdexcept>
#include <google/vcencoder.h>
#include <google/vcdecoder.h>


namespace Split {

    std::string DeltaCompressor::encode(const std::string& baseBytes, const std::string& targetBytes) {
        std::string deltaOut;

        open_vcdiff::VCDiffEncoder encoder(
            baseBytes.data(), baseBytes.size()
        );
        encoder.SetFormatFlags(open_vcdiff::VCD_FORMAT_INTERLEAVED);

        if (!encoder.Encode(targetBytes.data(), targetBytes.size(), &deltaOut)) {
            throw std::runtime_error("Failed to encode delta: ");
        }

        return deltaOut;
    }

    std::string DeltaCompressor::decode(const std::string& baseBytes, const std::string& deltaBytes) {

        std::string targetOut;

        open_vcdiff::VCDiffDecoder decoder;

        if (!decoder.Decode(baseBytes.data(), baseBytes.size(), deltaBytes, &targetOut)) {
            throw std::runtime_error("Failed to decode delta.");
        }

        return targetOut;
    }

}