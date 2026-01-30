//
// Created by Ahmed Mustafa on 1/30/2026.
//

#ifndef SPLITVCS_OFFSETEXCLUDE_H
#define SPLITVCS_OFFSETEXCLUDE_H

#include <ostream>

namespace Split
{
    struct OffsetBound
    {
        size_t offset, length;

        OffsetBound(size_t offset, size_t length) : offset(offset), length(length) {}
    };

    bool operator < (OffsetBound lhs, OffsetBound rhs);
    void operator << (std::ostream& out, OffsetBound offsetExclude);
}

#endif //SPLITVCS_OFFSETEXCLUDE_H