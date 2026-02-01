//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include "PsdEncoder.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>

#include "features/dissolve/AssetDissolver.h"
#include "PsdSdk/PsdChannelType.h"
#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdLayerMaskSection.h"
#include "PsdSdk/PsdMallocAllocator.h"
#include "PsdSdk/PsdNativeFile.h"
#include "PsdSdk/PsdParseDocument.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdExport.h"
#include "PsdSdk/PsdSyncFileReader.h"
#include "PsdSdk/PsdSyncFileUtil.h"
#include "utils/psd/ChannelUtils.h"
#include "utils/PsdLayersMetadata.h"
#include "utils/stream/OffsetBound.h"


namespace Split
{
    std::string PsdEncoder::encode(const std::string& base, const std::string& out)
    {
        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);
        psd::NativeFile metadataFile(&allocator);
        std::filesystem::path filepath(base);

        if (!file.OpenRead(filepath.wstring().c_str()))
            throw std::runtime_error("File not found");
        if (!metadataFile.OpenRead(filepath.wstring().c_str()))
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
            layerMetadata.storePath = base + layer->name.c_str();

            std::ofstream layerFile(layerMetadata.storePath, std::ios::binary | std::ios::trunc);
            layerFile.close();
            
            for (unsigned int j = 0; j < layer->channelCount; j++)
            {
                const auto channel = layer->channels[j];
                const OffsetBound exclude(channel.fileOffset, channel.size);

                // Save the raw buffer
                AssetDissolver extractor;
                extractor.slice(base, exclude, layerMetadata.storePath, std::ios::app);

                fileExcludeSet.insert(exclude);

                ChannelMetadata channelMetadata = {exclude};
                layerMetadata.channels.insert(channelMetadata);
            }

            psdLayerMetadata.layers.insert(layerMetadata);
        }

        AssetDissolver dissolver;
        dissolver.dissolve(base, out + "-psd", fileExcludeSet);

        // Cleanup
        psd::DestroyLayerMaskSection(layerMask, &allocator);
        psd::DestroyDocument(document, &allocator);
        file.Close();
        metadataFile.Close();

        std::ofstream metadataOut(out,std::ios::binary);
        metadataOut << psdLayerMetadata;

        metadataOut.close();

        return out;
    }

    std::string PsdEncoder::encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out)
    {
        return "";
    }

    void PsdEncoder::encode(const std::istream& v1, const std::istream& v2, std::ostream& out)
    {
    }
} // Split