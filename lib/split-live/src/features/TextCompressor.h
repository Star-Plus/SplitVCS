//
// Created by Ahmed Mustafa on 1/23/2026.
//

#ifndef SPLITVCS_TEXTCOMPRESSOR_H
#define SPLITVCS_TEXTCOMPRESSOR_H
#include "interfaces/ICompressor.h"

namespace Split
{
    class TextCompressor : public ICompressor
    {
    public:
        std::string encode(const std::string& src, const std::string& save, EncodeOptions options) override;
    };
}

#endif //SPLITVCS_TEXTCOMPRESSOR_H