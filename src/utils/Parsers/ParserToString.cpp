//
// Created by Ahmed Mustafa on 8/29/2025.
//

#include "ParserToString.h"

namespace Split {

    str ParserToString::fromSet(const std::set<str> &dataSet) {
        str result;
        for (const auto &item: dataSet) {
            result += item + "\n";
        }
        return result;
    }

    str ParserToString::fromVector(const std::vector<str> &dataSet) {
        str result;
        for (const auto &item: dataSet) {
            result += item + "\n";
        }
        return result;
    }

}