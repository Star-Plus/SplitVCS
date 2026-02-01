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

        // Get the Layer Mask Info Section offset - this is where layer records begin
        const uint64_t layerMaskSectionOffset = document->layerMaskInfoSection.offset;
        psdLayerMetadata.layerMaskSectionSizeOffset = layerMaskSectionOffset;

        // Read the Layer Info Section Length (first 4 bytes of layer mask section)
        psd::SyncFileReader initialReader(&metadataFile);
        initialReader.SetPosition(layerMaskSectionOffset);

        // Read past the section length (4 bytes) and layer count (2 bytes) to find first layer record
        psd::SyncFileReader sectionReader(&metadataFile);
        sectionReader.SetPosition(layerMaskSectionOffset);


        const uint32_t layerInfoSectionLength = psd::fileUtil::ReadFromFileBE<uint32_t>(sectionReader);
        psdLayerMetadata.layerMaskSectionSize = layerInfoSectionLength;
        std::cout << "Layer info section length: " << layerInfoSectionLength << std::endl;

        const int16_t layerCountSigned = psd::fileUtil::ReadFromFileBE<int16_t>(sectionReader);
        (void)layerInfoSectionLength;
        (void)layerCountSigned;

        // Current position is now at the start of the first layer record
        uint64_t currentLayerRecordOffset = sectionReader.GetPosition();

        for (unsigned int i = 0; i < layerMask->layerCount; i++)
        {
            psd::Layer* layer = &layerMask->layers[i];

            auto exportedLayers = psdAdapter.pixelsToMat(document, file, allocator, layer);
            const std::string savePrefix = out + "-" + layer->name.c_str();
            cv::imwrite(savePrefix + ".webp", exportedLayers.rgba, {cv::IMWRITE_WEBP_QUALITY, 101});

            LayerMetadata layerMetadata;
            layerMetadata.name = layer->name.c_str();

            // Calculate where each channel's size field is stored in the file:
            // Layer record structure:
            //   - top, left, bottom, right (4 int32_t = 16 bytes)
            //   - channel count (1 uint16_t = 2 bytes)
            //   - for each channel j:
            //       - channel type (1 int16_t = 2 bytes)
            //       - channel size (1 uint32_t = 4 bytes) ← WE NEED THIS OFFSET

            std::ofstream layerFile(out + layer->name.c_str(), std::ios::binary | std::ios::trunc);
            layerFile.close();
            
            for (unsigned int j = 0; j < 4 && j < layer->channelCount; j++)
            {
                const auto channel = layer->channels[j];
                const OffsetBound exclude(channel.fileOffset, channel.size);

                // Save the raw buffer
                AssetDissolver extractor;
                extractor.slice(base, exclude, out + layer->name.c_str(), std::ios::app);

                fileExcludeSet.insert(exclude);

                // Read compression type from file
                psd::SyncFileReader reader(&metadataFile);
                reader.SetPosition(channel.fileOffset);
                const auto compressionType = psd::fileUtil::ReadFromFileBE<uint16_t>(reader);

                // Calculate the offset where this channel's size field is stored:
                // currentLayerRecordOffset + 16 (coords) + 2 (channel count) + j*6 (previous channels) + 2 (skip type field)
                const size_t channelSizeFieldOffset = currentLayerRecordOffset + 16 + 2 + (j * 6) + 2;

                // Check if this channel is the alpha/transparency channel
                const bool isAlpha = (channel.type == psd::channelType::TRANSPARENCY_MASK);

                ChannelMetadata channelMetadata = {exclude, compressionType, channelSizeFieldOffset, isAlpha};
                layerMetadata.channels.insert(channelMetadata);
            }

            // Move to next layer record:
            // We need to skip past this entire layer record to get to the next one
            // This includes: coords (16) + channel count (2) + channel info (6 * channelCount) + extra data
            // Since we can't easily calculate the total size, we'll read the current position from the reader
            psd::SyncFileReader layerReader(&metadataFile);
            layerReader.SetPosition(currentLayerRecordOffset);

            // Skip: top, left, bottom, right (16 bytes)
            layerReader.Skip(16);

            // Read and skip channel count (2 bytes)
            const uint16_t channelCount = psd::fileUtil::ReadFromFileBE<uint16_t>(layerReader);

            // Skip all channel info (6 bytes per channel: 2 for type + 4 for size)
            layerReader.Skip(channelCount * 6);

            // Skip: blend signature (4) + blend mode (4) + opacity (1) + clipping (1) + flags (1) + filler (1) = 12 bytes
            layerReader.Skip(12);

            // Read extra data length and skip it
            const uint32_t extraDataLength = psd::fileUtil::ReadFromFileBE<uint32_t>(layerReader);
            layerReader.Skip(extraDataLength);

            // Now we're at the start of the next layer record
            currentLayerRecordOffset = layerReader.GetPosition();

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