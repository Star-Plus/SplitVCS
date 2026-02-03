//
// Created by Ahmed Mustafa on 1/31/2026.
//

#ifndef SPLITVCS_ASSETCAST_H
#define SPLITVCS_ASSETCAST_H

#include <string>
#include <set>

#include "BlockUnit.h"

namespace Split
{
    class AssetBlockBuilder
    {
    public:
        void combine(
            const std::string& building,
            std::set<BlockUnit>& blocks,
            const std::string& out
            );
    };
} // Split

#endif //SPLITVCS_ASSETCAST_H