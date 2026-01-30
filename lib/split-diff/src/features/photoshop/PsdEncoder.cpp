//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include "PsdEncoder.h"

#include <filesystem>

#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdLayerMaskSection.h"
#include "PsdSdk/PsdMallocAllocator.h"
#include "PsdSdk/PsdNativeFile.h"
#include "PsdSdk/PsdParseDocument.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdChannelType.h"
#include "PsdSdk/PsdExport.h"
#include "PsdSdk/PsdLayerMask.h"
#include "utils/psd/ChannelUtils.h"


namespace Split
{
    std::string PsdEncoder::encode(const std::string& base, const std::string& out)
    {
        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);
        std::filesystem::path filepath(base);

        if (!file.OpenRead(filepath.wstring().c_str()))
            throw std::runtime_error("File not found");

        psd::Document* document = psd::CreateDocument(&file, &allocator);
        psd::LayerMaskSection* layerMask = psd::ParseLayerMaskSection(document, &file, &allocator);

        // 1. Create the Export Document (Matching original dimensions and bit depth)
        psd::ExportDocument* exportDoc = psd::CreateExportDocument(&allocator,
            document->width, document->height, document->bitsPerChannel, psd::exportColorMode::RGB);

        for (unsigned int i = 0; i < layerMask->layerCount; i++)
        {
            psd::Layer* layer = &layerMask->layers[i];

            auto exportedLayers = psdAdapter.pixelsToMat(document, file, allocator, layer);
            const std::string savePrefix = out + "-" + layer->name.c_str();
            cv::imwrite(savePrefix + ".webp", exportedLayers.rgba, {cv::IMWRITE_WEBP_QUALITY, 101});
            cv::imwrite(savePrefix + "-mask.webp", exportedLayers.mask, {cv::IMWRITE_WEBP_QUALITY, 101});


            const unsigned int layerIndex = psd::AddLayer(exportDoc, &allocator, layer->name.c_str());
            const int layerW = layer->right - layer->left;
            const int layerH = layer->bottom - layer->top;
            const size_t bufferSize = layerW * layerH * (document->bitsPerChannel / 8);

            // Create a zeroed buffer
            void* zeroBuffer = allocator.Allocate(bufferSize, 16u);
            memset(zeroBuffer, 0, bufferSize);

            // Update standard channels
            psd::UpdateLayer(exportDoc, &allocator, layerIndex, psd::exportChannel::RED,
                             layer->left, layer->top, layer->right, layer->bottom, static_cast<const uint8_t*>(zeroBuffer), psd::compressionType::RLE);
            psd::UpdateLayer(exportDoc, &allocator, layerIndex, psd::exportChannel::GREEN,
                             layer->left, layer->top, layer->right, layer->bottom, static_cast<const uint8_t*>(zeroBuffer), psd::compressionType::RLE);
            psd::UpdateLayer(exportDoc, &allocator, layerIndex, psd::exportChannel::BLUE,
                             layer->left, layer->top, layer->right, layer->bottom, static_cast<const uint8_t*>(zeroBuffer), psd::compressionType::RLE);

            if (ChannelUtils::FindChannel(layer, psd::channelType::TRANSPARENCY_MASK) != ChannelUtils::CHANNEL_NOT_FOUND)
            {
                psd::UpdateLayer(exportDoc, &allocator, layerIndex, psd::exportChannel::ALPHA,
                                 layer->left, layer->top, layer->right, layer->bottom, static_cast<const uint8_t*>(zeroBuffer), psd::compressionType::RLE);
            }

            // Zero out the Layer Mask if it exists
            if (layer->layerMask)
            {
                const int maskW = layer->layerMask->right - layer->layerMask->left;
                const int maskH = layer->layerMask->bottom - layer->layerMask->top;
                const size_t maskSize = maskW * maskH * (document->bitsPerChannel / 8);

                void* zeroMaskBuffer = allocator.Allocate(maskSize, 16u);
                memset(zeroMaskBuffer, 0, maskSize);

                psd::UpdateLayer(exportDoc, &allocator, layerIndex, psd::exportChannel::ALPHA,
                                 layer->layerMask->left, layer->layerMask->top,
                                 layer->layerMask->right, layer->layerMask->bottom, static_cast<const uint8_t*>(zeroMaskBuffer), psd::compressionType::RLE);

                allocator.Free(zeroMaskBuffer);
            }

            allocator.Free(zeroBuffer);
        }

        // 2. Write the Zeroed PSD
        std::filesystem::path outPath(out);
        psd::NativeFile outFile(&allocator);
        if (outFile.OpenWrite(outPath.wstring().c_str()))
        {
            psd::WriteDocument(exportDoc, &allocator, &outFile);
            outFile.Close();
        }

        // Cleanup
        psd::DestroyExportDocument(exportDoc, &allocator);
        psd::DestroyLayerMaskSection(layerMask, &allocator);
        psd::DestroyDocument(document, &allocator);
        file.Close();

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