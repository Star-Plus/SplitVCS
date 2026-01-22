//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include "PackOptimizer.h"

#include "stores/ObjectStore.h"
#include <fstream>

#include "FsMetadata.h"

namespace Split
{
    size_t PackOptimizer::approximateDecodeSize(const str& rootPath, const std::shared_ptr<PackUnit>& leaf)
    {
        auto tempPack = leaf;
        auto tailPack = leaf;

        size_t estimatedSize = 0;

        const auto deltaStore = ObjectStore(rootPath, "/deltas");

        while (tempPack->baseRef != nullptr)
        {
            tailPack = tempPack;

            const auto deltaSize = FsMetadata::calculateFileSize(deltaStore.getPath() + "/" + tempPack->deltaHash) * getDecodeSizeFactor(tempPack->encodeType);
            estimatedSize += deltaSize;

            tempPack = tempPack->baseRef;
        }

        const auto blobStore = ObjectStore(rootPath, "/blobs");
        const auto baseBlobSize = FsMetadata::calculateFileSize(blobStore.getPath() + "/" + tempPack->hash);
        estimatedSize += baseBlobSize;

        return estimatedSize;
    }

    float PackOptimizer::getDecodeSizeFactor(const AssetType type)
    {
        switch (type)
        {
        case AssetType::BINARY:
            return 1.0;
        case AssetType::IMAGE:
            return 0.1;
        }

        return 1.2;
    }
}
