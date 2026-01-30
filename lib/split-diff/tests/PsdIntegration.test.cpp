//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include <gtest/gtest.h>
#include <PsdSdk/PsdMallocAllocator.h>
#include <PsdSdk/PsdNativeFile.h>

#include "PsdSdk/PsdChannelType.h"
#include "PsdSdk/PsdColorMode.h"
#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdImageResourcesSection.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdLayerMaskSection.h"
#include "PsdSdk/PsdParseDocument.h"
#include "PsdSdk/PsdParseImageResourcesSection.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "utils/psd/ChannelUtils.h"
#include "utils/psd/TrackingFile.h"

TEST(PsdIntegrationTest, FirstCall)
{
    const std::wstring srcPath = L"PSDs/sample.psd";

    psd::MallocAllocator allocator;
    psd::NativeFile file(&allocator);

    ASSERT_TRUE(file.OpenRead(srcPath.c_str()));

    psd::Document* document = psd::CreateDocument(&file, &allocator);
    ASSERT_TRUE(document);

    ASSERT_EQ(document->colorMode, psd::colorMode::RGB);

    psd::ImageResourcesSection* imageResources = psd::ParseImageResourcesSection(document, &file, &allocator);
    ASSERT_TRUE(imageResources);

    std::cout << "XMP Metadata: " << imageResources->xmpMetadata << std::endl;

    psd::DestroyImageResourcesSection(imageResources, &allocator);

    psd::LayerMaskSection* layerMask = psd::ParseLayerMaskSection(document, &file, &allocator);
    ASSERT_TRUE(layerMask);

    bool hasTransparentLayerMask = layerMask->hasTransparencyMask;

    for (unsigned int i = 0; i < layerMask->layerCount; i++)
    {
        psd::Layer* layer = &layerMask->layers[i];
        psd::ExtractLayer(document, &file, &allocator, layer);

        const unsigned int iR = Split::ChannelUtils::FindChannel(layer, psd::channelType::R);
        const unsigned int iG = Split::ChannelUtils::FindChannel(layer, psd::channelType::G);
        const unsigned int iB = Split::ChannelUtils::FindChannel(layer, psd::channelType::B);
        const unsigned int iA = Split::ChannelUtils::FindChannel(layer, psd::channelType::TRANSPARENCY_MASK);

        if (iR == Split::ChannelUtils::CHANNEL_NOT_FOUND ||
            iG == Split::ChannelUtils::CHANNEL_NOT_FOUND ||
            iB == Split::ChannelUtils::CHANNEL_NOT_FOUND) continue;

        void* R = Split::ChannelUtils::ExpandChannelToCanvas(document, &allocator, layer, &layer->channels[iR]);
        void* G = Split::ChannelUtils::ExpandChannelToCanvas(document, &allocator, layer, &layer->channels[iG]);
        void* B = Split::ChannelUtils::ExpandChannelToCanvas(document, &allocator, layer, &layer->channels[iR]);
        void* A = (iA != Split::ChannelUtils::CHANNEL_NOT_FOUND) ? Split::ChannelUtils::ExpandChannelToCanvas(document, &allocator, layer, &layer->channels[iR]) : nullptr;

        uint8_t* rgba = nullptr;

        if (A)
        {
            rgba = Split::ChannelUtils::CreateInterleavedImage<uint8_t>(
                &allocator, R, G, B, A,
                document->width, document->height
            );
        } else
        {
            rgba = Split::ChannelUtils::CreateInterleavedImage<uint8_t>(
                &allocator, R, G, B,
                document->width, document->height
            );
        }

        std::cout << layer->name.c_str() << std::endl;
        std::cout << "Final layer size: " << sizeof(rgba) << std::endl;

        allocator.Free(R);
        allocator.Free(G);
        allocator.Free(B);
        allocator.Free(A);
        allocator.Free(rgba);
    }
}