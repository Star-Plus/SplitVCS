//
// Created by Ahmed Mustafa on 2/3/2026.
//

#ifndef SPLITVCS_PSDULTRAVIOLET_H
#define SPLITVCS_PSDULTRAVIOLET_H
#include "interfaces/ICompressor.h"

namespace Split
{
    class PsdUltraviolet : ICompressor
    {
    public:
        std::string encode(const std::string& src, const std::string& save, EncodeOptions options = {}) override;
    };
} // Split

#endif //SPLITVCS_PSDULTRAVIOLET_H