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

        std::cout << "Initial layerMaskSectionSize: " << metadata.layerMaskSectionSize << std::endl;

        AssetBlockBuilder assetBuilder;
        PsdMatAdapter adapter;

        std::set<BlockUnit> blocks;

        std::vector<size_t> allChannelSizeFieldOffsets;
        std::vector<uint32_t> allNewChannelSizes;

        for (auto& layer : metadata.layers)
        {
            auto channel = layer.channels.begin();

            auto image = cv::imread(base + "-" + layer.name.c_str() + ".webp", cv::IMREAD_UNCHANGED);

            std::vector<uint16_t> compressionTypes;
            std::vector<bool> alphaFlags;

            for (auto& c : layer.channels)
            {
                compressionTypes.push_back(0);
                alphaFlags.push_back(c.isAlpha);
            }

            auto planers = adapter.matToPsdBuffer(image, compressionTypes, alphaFlags);

            for (int i = 0; i < layer.channels.size(); i++)
            {
                const auto startOffset = channel->offset.offset;
                const auto blockSize = channel->offset.length;

                BlockUnit block = {planers.at(i).stream, OffsetBound(startOffset, blockSize, planers.at(i).size)};
                blocks.insert(block);

                allChannelSizeFieldOffsets.push_back(channel->channelSizeFieldOffset);
                allNewChannelSizes.push_back(planers.at(i).size);


                metadata.layerMaskSectionSize += planers.at(i).size - channel->offset.length;

                ++channel;
            }
        }

        std::cout << "Final layerMaskSectionSize: " << metadata.layerMaskSectionSize << std::endl;

        const std::string basePsdFile = base + "-psd";

        PsdChannelSizeUpdater::updateChannelSizes(
            base,
            allChannelSizeFieldOffsets,
            allNewChannelSizes,
            metadata.layerMaskSectionSizeOffset,
            metadata.layerMaskSectionSize
        );

        assetBuilder.combine(basePsdFile, blocks, out);
    }
} // Split