//
// Created by Ahmed Mustafa on 1/30/2026.
//

#ifndef SPLITVCS_PSDDECODER_H
#define SPLITVCS_PSDDECODER_H
#include "interfaces/IDecoder.h"
#include "utils/compress/Bit7Archive.h"

namespace Split
{

    struct PsdDecodeResponse
    {
        std::string metadata;
        std::string hollowed;
        std::vector<std::string> components;
    };

    class PsdDecoder : public Decoder
    {
    public:
        void decode(const std::string& base, std::stack<std::string>& deltas, std::string& out) override;
        
    private:
        Bit7Archive psdArchive;
    };
} // Split

#endif //SPLITVCS_PSDDECODER_H