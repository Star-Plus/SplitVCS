#include "LiveEncoder.h"

#include <stdexcept>

namespace Split {

    LiveEncoder::LiveEncoder() = default;

    LiveEncoder::~LiveEncoder() = default;

    std::string LiveEncoder::encode(const std::string& src, const std::string& save, const EncodeOptions options) const {
        const auto compressor = compressorFactory.getCompressor(options.type);

        if (compressor == nullptr)
            throw std::runtime_error("Compressor cannot be null");

        return compressor->encode(src, save, options);
    }
}
