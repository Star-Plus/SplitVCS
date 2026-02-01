//
// Created by Ahmed Mustafa on 1/30/2026.
//

#include "OffsetBound.h"

namespace Split
{

    bool operator < (const OffsetBound lhs, const OffsetBound rhs)
    {
        return lhs.offset < rhs.offset;
    }

    void operator << (std::ostream& out, const OffsetBound offsetExclude)
    {
        out << offsetExclude.offset << "\n" << offsetExclude.length;
    }

}