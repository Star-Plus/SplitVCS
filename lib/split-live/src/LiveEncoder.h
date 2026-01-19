#pragma once
#include "components/CompressorFactory.h"

namespace Split {

    class LiveEncoder {
        public:
            LiveEncoder();
            ~LiveEncoder();

            void encode(const std::string& , const std::string&, EncodeOptions = {}) const;

    private:
        CompressorFactory compressorFactory;
    };

}
