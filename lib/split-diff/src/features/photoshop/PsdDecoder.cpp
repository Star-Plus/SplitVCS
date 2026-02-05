//
// Created by Ahmed Mustafa on 1/30/2026.
//

#include <filesystem>
#include "PsdDecoder.h"
#include <fstream>
#include "features/binary/ByteDecoder.h"
#include "features/dissolve/AssetBlockBuilder.h"
#include "utils/DecodeUtils.h"
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
        auto baseWithoutExtension = base;
        PathUtils::removeSuffix(baseWithoutExtension, ".7z");

        // Modify out if it ends with .psd
        PathUtils::removeSuffix(out, ".psd");
        std::filesystem::create_directories(out);

        std::vector foldersToCleanUp = {out, baseWithoutExtension};
        copyToVector(deltas, foldersToCleanUp);

        {
            {
                // Extract base archive
                this->psdArchive.ExtractArchive(base, baseWithoutExtension + "/");
                // Extract all delta archives if deltas are not empty
                if (!deltas.empty())
                {
                    DecodeUtils::extractDeltaArchives(deltas, psdArchive);
                }
            }

            // Read metadata
            std::string metadataParentPath = deltas.empty() ? base : deltas.top();
            PathUtils::removeSuffix(metadataParentPath, ".7z");
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
                const std::string basePath = baseWithoutExtension + "/" + suffix;

                // Decode layer: if deltas are empty, base is returned as-is
                std::stack<std::string> layerDeltas;
                if (!deltas.empty())
                {
                    // Copy delta stack with layer suffix
                    layerDeltas = DecodeUtils::copyStackSuffixEdited(deltas, suffix, ".7z");
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
            const std::string baseSkeletonPath = baseWithoutExtension + suffix;
            std::string tmpSkeletonPath = out + "/skeleton.tmp";

            std::stack<std::string> skeletonDeltaStack;
            if (!deltas.empty())
            {
                skeletonDeltaStack = DecodeUtils::copyStackSuffixEdited(deltas, suffix);
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