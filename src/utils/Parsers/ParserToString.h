//
// Created by Ahmed Mustafa on 8/29/2025.
//

#ifndef PARSERTOSTRING_H
#define PARSERTOSTRING_H

#include <set>
#include <vector>

#include "core/Alias.h"

namespace Split {
    class ParserToString {

    public:
        static str fromSet(const std::set<str>& dataSet);
        static str fromVector(const std::vector<str>& dataSet);
    };
}

#endif //PARSERTOSTRING_H
