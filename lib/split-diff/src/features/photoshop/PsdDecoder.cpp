//
// Created by Ahmed Mustafa on 1/30/2026.
//

#include <filesystem>
#include "PsdDecoder.h"
#include <fstream>
#include <iostream>
#include <queue>

#include "features/binary/ByteDecoder.h"
#include "features/dissolve/AssetBlockBuilder.h"
#include "utils/PsdLayersMetadata.h"
#include "utils/PsdLayerMetadataParser.h"
#include "utils/compress/Bit7Archive.h"
#include "utils/path/PathUtils.h"

namespace psd
{
    struct ExportDocument;
    struct LayerMaskSection;;
    struct Document;
}

namespace Split
{
    template <typename T>
    std::stack<T> copyStackEditedWithSuffix(std::stack<T> s1, const T& suffix)
    {
        std::queue<T> tempQueue;

        while (!s1.empty())
        {
            tempQueue.push(s1.top()+suffix);
            s1.pop();
        }

        std::stack<T> stack;
        while (!tempQueue.empty())
        {
            stack.push(tempQueue.front());
            tempQueue.pop();
        }

        return stack;
    }

    void PsdDecoder::extractDeltaArchives(std::stack<std::string> deltas) const
    {
        while (!deltas.empty())
        {
            auto delta = deltas.top();
            deltas.pop();
            psdArchive.ExtractArchive(delta + ".7z", delta + "/");
        }
    }

    void copyToVector(std::stack<std::string> stk, std::vector<std::string>& vec)
    {
        while (!stk.empty())
        {
            vec.push_back(stk.top());
            stk.pop();
        }
    }

    void PsdDecoder::decode(const std::string& base, std::stack<std::string>& deltas, std::string& out)
    {
        // Modify out if it ends with .psd
        PathUtils::removeSuffix(out, ".psd");
        std::filesystem::create_directories(out);

        std::vector foldersToCleanUp = {out};
        copyToVector(deltas, foldersToCleanUp);

        {
            // Extract all delta archives if deltas are not empty
            if (!deltas.empty())
            {
                extractDeltaArchives(deltas);
            }

            // Read metadata
            std::string metadataParentPath = deltas.empty() ? base : deltas.top();
            std::fstream metadataFile(metadataParentPath + "/metadata", std::ios::in);
            if (!metadataFile.is_open()) throw std::ios_base::failure("Cannot open metadata file");

            PsdLayerMetadataParser metadataParser;
            auto metadata = metadataParser.parse(metadataFile);
            metadataFile.close();

            ByteDecoder byteDecoder;
            AssetBlockBuilder assetBuilder;
            std::set<BlockUnit> blocks;

            // Process each layer and decode with deltas (or base only if deltas are empty)
            for (auto& layer : metadata.layers)
            {
                if (layer.channels.empty()) continue;

                const auto startChannel = layer.channels.begin();
                const auto endChannel = layer.channels.rbegin();

                const auto startOffset = startChannel->offset.offset;
                const auto blockSize = endChannel->offset.offset - startOffset + endChannel->offset.length;

                const std::string suffix = "/" + layer.name + ".layer";
                const std::string basePath = base + "/" + suffix;

                // Decode layer: if deltas are empty, base is returned as-is
                std::stack<std::string> layerDeltas;
                if (!deltas.empty())
                {
                    // Copy delta stack with layer suffix
                    layerDeltas = copyStackEditedWithSuffix(deltas, suffix);
                }

                std::string tmpDecodedPath = out + "/" + suffix + ".tmp";
                byteDecoder.decode(basePath, layerDeltas, tmpDecodedPath);

                // Open decoded layer stream
                auto stream = std::make_shared<std::ifstream>(tmpDecodedPath, std::ios::binary);
                if (!stream->is_open()) throw std::runtime_error("Cannot open decoded layer file: " + tmpDecodedPath);

                BlockUnit block = {stream, OffsetBound(startOffset, blockSize)};
                blocks.insert(block);
            }

            // Decode skeleton with its deltas (or base only if deltas are empty)
            const std::string suffix = "/skeleton";
            const std::string baseSkeletonPath = base + suffix;
            std::string tmpSkeletonPath = out + "/skeleton.tmp";

            std::stack<std::string> skeletonDeltaStack;
            if (!deltas.empty())
            {
                skeletonDeltaStack = copyStackEditedWithSuffix(deltas, suffix);
            }

            byteDecoder.decode(baseSkeletonPath, skeletonDeltaStack, tmpSkeletonPath);

            // Combine decoded skeleton with decoded layer blocks
            assetBuilder.combine(tmpSkeletonPath, blocks, out + ".psd");
        }

        // Cleanup temporary files
        for (auto& fd : foldersToCleanUp)
        {
            std::filesystem::remove_all(fd);
        }
    }
} // Split