//
// Created by Ahmed Mustafa on 1/29/2026.
//

#ifndef SPLITVCS_PSDENCODER_H
#define SPLITVCS_PSDENCODER_H
#include "interfaces/IEncoder.h"
#include "utils/PsdMatAdapter.h"

namespace Split
{
    class PsdEncoder : public Encoder
    {
    public:
        std::string encode(const std::string& base, const std::string& out) override;
        std::string encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out) override;
        void encode(const std::istream& v1, const std::istream& v2, std::ostream& out) override;
    private:
        PsdMatAdapter psdAdapter;
    };
} // Split

#endif //SPLITVCS_PSDENCODER_H