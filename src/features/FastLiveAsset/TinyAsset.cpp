//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include "TinyAsset.h"

#include <utility>

#include "stores/Pack.h"
#include <fstream>

namespace Split
{
    TinyAsset::TinyAsset(str  rootPath) : rootPath(std::move(rootPath))
    {}

    void TinyAsset::encodeAsset(const str& assetHash)
    {
        Pack pack(this->rootPath);

        std::fstream outFile(this->rootPath + "./split/tmp/decode"+assetHash);

        pack.decode(assetHash, outFile);


    }
} // Split