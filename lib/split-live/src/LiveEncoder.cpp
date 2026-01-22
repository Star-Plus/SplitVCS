#include "LiveEncoder.h"

#include <stdexcept>

namespace Split {

    LiveEncoder::LiveEncoder() {
        
    }

    LiveEncoder::~LiveEncoder() {

    }

    void LiveEncoder::encode(const std::string& src, const std::string& save, const EncodeOptions options) const {
        const auto type = assetTypeFromPath(src);
        const auto compressor = compressorFactory.getCompressor(type);

        if (compressor == nullptr)
            throw std::runtime_error("Compressor cannot be null");

        compressor->encode(src, save, options);
    }
}
