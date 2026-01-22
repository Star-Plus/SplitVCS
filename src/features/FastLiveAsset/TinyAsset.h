//
// Created by Ahmed Mustafa on 1/22/2026.
//

#ifndef SPLITVCS_TINYASSET_H
#define SPLITVCS_TINYASSET_H
#include "core/Alias.h"

namespace Split
{
    class TinyAsset
    {
    public:
        explicit TinyAsset(str  rootPath);

        void encodeAsset(const str& assetHash);

    private:
        str rootPath;
    };
} // Split

#endif //SPLITVCS_TINYASSET_H