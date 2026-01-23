#pragma once
#include "components/CompressorFactory.h"

namespace Split {

    class LiveEncoder {
        public:
            LiveEncoder();
            ~LiveEncoder();

            std::string encode(const std::string& , const std::string&, EncodeOptions = {}) const;

    private:
        CompressorFactory compressorFactory;
    };

}
