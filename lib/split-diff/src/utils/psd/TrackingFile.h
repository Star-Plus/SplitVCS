//
// Created by Ahmed Mustafa on 1/29/2026.
//

#ifndef SPLITVCS_TRACKINGFILE_H
#define SPLITVCS_TRACKINGFILE_H
#include "PsdSdk/PsdNativeFile.h"

#include <vector>

namespace Split
{

    class TrackingFile : public psd::NativeFile
    {
    public:
        explicit TrackingFile(psd::Allocator* allocator) : NativeFile(allocator) {}

        struct ReadLog
        {
            uint64_t offset;
            uint32_t size;
        };

        std::vector<ReadLog> reads;

        ReadOperation Read(void* buffer, uint32_t count, uint64_t position) override
        {
            reads.push_back(ReadLog{ position, count });
            return NativeFile::Read(buffer, count, position);
        }
    };

}

#endif //SPLITVCS_TRACKINGFILE_H
