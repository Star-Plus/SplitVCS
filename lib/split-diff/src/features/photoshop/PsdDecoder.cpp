//
// Created by Ahmed Mustafa on 1/30/2026.
//

#include <filesystem>
#include "PsdDecoder.h"
#include <fstream>
#include <iostream>

#include "features/dissolve/AssetBlockBuilder.h"
#include "utils/PsdLayersMetadata.h"
#include "utils/PsdLayerMetadataParser.h"
#include "utils/PsdMatAdapter.h"
#include "utils/psd/PsdChannelSizeUpdater.h"

namespace psd
{
    struct ExportDocument;
    struct LayerMaskSection;;
    struct Document;
}

namespace Split
{
    void PsdDecoder::decode(const std::string& base, std::stack<std::string>& deltas, std::string& out)
    {
        std::fstream metadataFile(base, std::ios::in);
        if (!metadataFile.is_open()) throw std::ios_base::failure("Cannot open metadata file");

        PsdLayerMetadataParser metadataParser;
        auto metadata = metadataParser.parse(metadataFile);

        AssetBlockBuilder assetBuilder;

        std::set<BlockUnit> blocks;

        for (auto& layer : metadata.layers)
        {
            const auto startChannel = layer.channels.begin();
            const auto endChannel = layer.channels.rbegin();

            const auto startOffset = startChannel->offset.offset;
            const auto blockSize = endChannel->offset.offset - startOffset + endChannel->offset.length;

            auto stream = std::make_shared<std::ifstream>(layer.storePath, std::ios::binary);

            BlockUnit block = {stream, OffsetBound(startOffset, blockSize)};
            blocks.insert(block);
        }

        const std::string basePsdFile = base + "-psd";

        assetBuilder.combine(basePsdFile, blocks, out);
    }
} // Split