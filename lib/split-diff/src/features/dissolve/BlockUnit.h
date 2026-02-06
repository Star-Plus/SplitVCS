//
// Created by Ahmed Mustafa on 1/31/2026.
//

#ifndef SPLITVCS_BLOCKUNIT_H
#define SPLITVCS_BLOCKUNIT_H

#include <istream>
#include <memory>

#include "utils/stream/OffsetBound.h"

namespace Split
{
    struct BlockUnit
    {
        std::shared_ptr<std::istream> stream;
        OffsetBound offset;
    };

    inline bool operator < (const BlockUnit& lhs, const BlockUnit& rhs)
    {
        return lhs.offset < rhs.offset;
    }
}

#endif //SPLITVCS_BLOCKUNIT_H