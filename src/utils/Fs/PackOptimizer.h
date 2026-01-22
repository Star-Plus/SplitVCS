//
// Created by Ahmed Mustafa on 1/22/2026.
//

#ifndef SPLITVCS_PACKOPTIMIZER_H
#define SPLITVCS_PACKOPTIMIZER_H
#include "stores/Pack.h"

namespace Split
{
    class PackOptimizer
    {
    public:
        static size_t approximateDecodeSize(const str& rootPath, const std::shared_ptr<PackUnit>& leaf);
    private:
        static float getDecodeSizeFactor(AssetType);
    };
}



#endif //SPLITVCS_PACKOPTIMIZER_H