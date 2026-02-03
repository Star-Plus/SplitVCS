//
// Created by Ahmed Mustafa on 1/31/2026.
//

#ifndef SPLITVCS_ASSETDISSOLVER_H
#define SPLITVCS_ASSETDISSOLVER_H
#include "utils/stream/OffsetBound.h"
#include <set>

namespace Split
{
    class AssetDissolver
    {
    public:
        void dissolve(const std::string& base, const std::string& out, std::set<OffsetBound>& excludes);
        void slice(const std::string& base, const OffsetBound& excludes, const std::string& out, std::ios::openmode mode = std::ios::trunc);
    };
} // Split

#endif //SPLITVCS_ASSETDISSOLVER_H