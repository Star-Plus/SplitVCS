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
        size_t offset = 0, length = 0, updatedLength = 0;

        OffsetBound() {}
        OffsetBound(const size_t offset, const size_t length) : offset(offset), length(length), updatedLength(length) {}
        OffsetBound(const size_t offset, const size_t length, const size_t updatedLength) :
        offset(offset), length(length), updatedLength(updatedLength) {}
    };

    bool operator < (OffsetBound lhs, OffsetBound rhs);
    void operator << (std::ostream& out, OffsetBound offsetExclude);
}

#endif //SPLITVCS_OFFSETEXCLUDE_H