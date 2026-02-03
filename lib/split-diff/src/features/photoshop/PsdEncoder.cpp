//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include "PsdEncoder.h"

#include <filesystem>
#include <fstream>
#include <queue>
#include <set>

#include "PsdDecoder.h"
#include "features/binary/ByteEncoder.h"
#include "features/dissolve/AssetDissolver.h"
#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdLayerMaskSection.h"
#include "PsdSdk/PsdMallocAllocator.h"
#include "PsdSdk/PsdNativeFile.h"
#include "PsdSdk/PsdParseDocument.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdExport.h"
#include "utils/psd/ChannelUtils.h"
#include "utils/PsdLayersMetadata.h"
#include "utils/compress/Bit7Archive.h"
#include "utils/hashing/RandomHash.h"
#include "utils/stream/OffsetBound.h"


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

    void PsdEncoder::extractDeltaArchives(std::stack<std::string> deltas) const
    {
        while (!deltas.empty())
        {
            auto delta = deltas.top();
            psdArchive.ExtractArchive(delta + ".7z", delta + "/");
        }
    }

    std::string PsdEncoder::encode(const std::string& base, const std::string& out)
    {
        std::filesystem::create_directories(out);

        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);
        std::filesystem::path filepath(base);

        if (!file.OpenRead(filepath.wstring().c_str()))
            throw std::runtime_error("File not found");

        psd::Document* document = psd::CreateDocument(&file, &allocator);
        psd::LayerMaskSection* layerMask = psd::ParseLayerMaskSection(document, &file, &allocator);

        PsdLayersMetadata psdLayerMetadata;
        std::set<OffsetBound> fileExcludeSet;

        for (unsigned int i = 0; i < layerMask->layerCount; i++)
        {
            psd::Layer* layer = &layerMask->layers[i];

            LayerMetadata layerMetadata;
            layerMetadata.name = layer->name.c_str();
            layerMetadata.storePath = out + "/" + layer->name.c_str() + ".layer";

            std::ofstream layerFile(layerMetadata.storePath, std::ios::binary | std::ios::trunc);
            layerFile.close();
            
            for (unsigned int j = 0; j < layer->channelCount; j++)
            {
                const auto channel = layer->channels[j];
                const OffsetBound exclude(channel.fileOffset, channel.size);

                // Save the raw buffer
                extractor.slice(base, exclude, layerMetadata.storePath, std::ios::app);

                fileExcludeSet.insert(exclude);

                ChannelMetadata channelMetadata = {exclude};
                layerMetadata.channels.insert(channelMetadata);
            }

            psdLayerMetadata.layers.insert(layerMetadata);
        }

        AssetDissolver dissolver;
        dissolver.dissolve(base, out + "/skeleton", fileExcludeSet);

        // Cleanup
        psd::DestroyLayerMaskSection(layerMask, &allocator);
        psd::DestroyDocument(document, &allocator);
        file.Close();

        std::ofstream metadataOut(out + "/metadata",std::ios::binary);
        metadataOut << psdLayerMetadata;
        metadataOut.close();

        return out;
    }

    std::string PsdEncoder::encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out)
    {
        std::filesystem::create_directories(out);

        ByteEncoder byteEncoder;

        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);
        std::filesystem::path filepath(v2);

        if (!file.OpenRead(filepath.wstring().c_str()))
            throw std::runtime_error("File not found");

        psd::Document* document = psd::CreateDocument(&file, &allocator);
        psd::LayerMaskSection* layerMask = psd::ParseLayerMaskSection(document, &file, &allocator);

        PsdLayersMetadata psdLayerMetadata;
        std::set<OffsetBound> fileExcludeSet;

        const auto assetHash = RandomHash::generateHash(16);

        // Extract archives
        extractDeltaArchives(deltas);

        for (unsigned int i = 0; i < layerMask->layerCount; i++)
        {
            psd::Layer* layer = &layerMask->layers[i];

            LayerMetadata layerMetadata;
            layerMetadata.name = layer->name.c_str();
            const std::string suffix = "/" + layerMetadata.name + ".layer";
            const std::string tmpSlicePath = out + suffix + ".tmp";

            std::string outDeltaPath = out + suffix;
            layerMetadata.storePath = outDeltaPath;


            // Copy delta stack but adding layer storePath as suffix
            std::stack<std::string> layerDeltas = copyStackEditedWithSuffix(deltas, suffix);


            std::ofstream layerFile(layerMetadata.storePath, std::ios::binary | std::ios::trunc);
            layerFile.close();

            for (unsigned int j = 0; j < layer->channelCount; j++)
            {
                const auto channel = layer->channels[j];
                const OffsetBound exclude(channel.fileOffset, channel.size);

                // Save the raw buffer
                extractor.slice(v2, exclude,tmpSlicePath , std::ios::app);

                // Encode layer deltas
                auto _ = byteEncoder.encode(base, layerDeltas, tmpSlicePath, outDeltaPath);

                // Remove tmp slice file

                fileExcludeSet.insert(exclude);

                ChannelMetadata channelMetadata = {exclude};
                layerMetadata.channels.insert(channelMetadata);
            }

            std::filesystem::remove(tmpSlicePath);

            psdLayerMetadata.layers.insert(layerMetadata);
        }


        // Dissolve main psd
        AssetDissolver dissolver;
        const std::string tmpSkeletonPath = out + "/skeleton.tmp";
        dissolver.dissolve(v2, tmpSkeletonPath, fileExcludeSet);
        // Copy deltas stack for skeleton suffix
        const std::string suffix = "/skeleton";
        auto skeletonDeltaStack = copyStackEditedWithSuffix(deltas, suffix);
        // Encode skeleton
        std::string outDeltaPath = out + suffix;
        byteEncoder.encode(base+"/skeleton", skeletonDeltaStack, tmpSkeletonPath, outDeltaPath);
        // Remove tmp file
        std::filesystem::remove(tmpSkeletonPath);

        // Cleanup
        psd::DestroyLayerMaskSection(layerMask, &allocator);
        psd::DestroyDocument(document, &allocator);
        file.Close();

        std::ofstream metadataOut(out + "/metadata",std::ios::binary);
        metadataOut << psdLayerMetadata;
        metadataOut.close();

        // Archive the output
        const std::string zipOutPath = out + ".7z";
        psdArchive.SaveDirToArchive(out + "/", zipOutPath);

        std::filesystem::remove_all(out);

        return out;
    }

} // Split