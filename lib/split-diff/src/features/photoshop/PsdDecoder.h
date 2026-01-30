//
// Created by Ahmed Mustafa on 1/30/2026.
//

#ifndef SPLITVCS_PSDDECODER_H
#define SPLITVCS_PSDDECODER_H
#include "interfaces/IDecoder.h"

namespace Split
{
    class PsdDecoder : public Decoder
    {
    public:
        void decode(const std::string& base, std::stack<std::string>& deltas, std::string& out) override;

    };
} // Split

#endif //SPLITVCS_PSDDECODER_H