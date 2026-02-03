//
// Created by Ahmed Mustafa on 1/29/2026.
//

#ifndef SPLITVCS_PSDENCODER_H
#define SPLITVCS_PSDENCODER_H
#include "features/dissolve/AssetDissolver.h"
#include "interfaces/IEncoder.h"
#include "utils/PsdMatAdapter.h"
#include "utils/compress/Bit7Archive.h"

namespace Split
{
    class PsdEncoder : public Encoder
    {
    public:
        std::string encode(const std::string& base, const std::string& out) override;
        std::string encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out) override;
    private:
        PsdMatAdapter psdAdapter;
        Bit7Archive psdArchive;
        AssetDissolver extractor;

        void extractDeltaArchives(std::stack<std::string> deltas) const;
    };
} // Split

#endif //SPLITVCS_PSDENCODER_H