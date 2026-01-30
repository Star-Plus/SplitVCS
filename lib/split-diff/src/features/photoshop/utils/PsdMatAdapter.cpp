//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include "PsdMatAdapter.h"

#include "PsdSdk/PsdAllocator.h"
#include "PsdSdk/PsdChannelType.h"
#include "PsdSdk/PsdLayerMask.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "utils/psd/ChannelUtils.h"

namespace Split
{

    ExportedLayer PsdMatAdapter::pixelsToMat(
        const psd::Document* document,
        psd::NativeFile& file,
        psd::Allocator& allocator,
        psd::Layer* layer
        )
    {
        psd::ExtractLayer(document, &file, &allocator, layer);

        const int w = layer->right - layer->left;
        const int h = layer->bottom - layer->top;

        const unsigned int iR = ChannelUtils::FindChannel(layer, psd::channelType::R);
        const unsigned int iG = ChannelUtils::FindChannel(layer, psd::channelType::G);
        const unsigned int iB = ChannelUtils::FindChannel(layer, psd::channelType::B);
        const unsigned int iA = ChannelUtils::FindChannel(layer, psd::channelType::TRANSPARENCY_MASK);

        if (iR == ChannelUtils::CHANNEL_NOT_FOUND ||
            iG == ChannelUtils::CHANNEL_NOT_FOUND ||
            iB == ChannelUtils::CHANNEL_NOT_FOUND) throw std::runtime_error("Asset not texture");

        const void* R = layer->channels[iR].data;
        const void* G = layer->channels[iG].data;
        const void* B = layer->channels[iB].data;
        const void* A = iA != ChannelUtils::CHANNEL_NOT_FOUND ? layer->channels[iA].data : nullptr;

        uint8_t* rgba = nullptr;

        if (A)
        {
            rgba = ChannelUtils::CreateInterleavedImage<uint8_t>(
                &allocator, R, G, B, A,
                w, h
            );
        } else
        {
            rgba = ChannelUtils::CreateInterleavedImage<uint8_t>(
                &allocator, R, G, B,
                w, h
            );
        }

        cv::Mat fullRGBA = cv::Mat(h, w, CV_8UC4, rgba).clone();
        cv::cvtColor(fullRGBA, fullRGBA, cv::COLOR_RGBA2BGRA);
        allocator.Free(rgba);

        const uint8_t background = layer->layerMask ? layer->layerMask->defaultColor : 255;
        cv::Mat maskMat(h, w, CV_8UC1, cv::Scalar(background));

        const bool hasValidMask =
            layer->layerMask &&
            layer->layerMask->data &&
            layer->layerMask->right > layer->layerMask->left &&
            layer->layerMask->bottom > layer->layerMask->top;

        if (hasValidMask)
        {
            const auto& mask = *layer->layerMask;
            const int maskW = mask.right - mask.left;
            const int maskH = mask.bottom - mask.top;
            const auto maskData = static_cast<const uint8_t*>(mask.data);

            for (int y = 0; y < h; ++y)
            {
                const int docY = layer->top + y;
                const int maskY = docY - mask.top;
                if (maskY < 0 || maskY >= maskH) continue;

                for (int x = 0; x < w; ++x)
                {
                    const int docX = layer->left + x;
                    const int maskX = docX - mask.left;
                    if (maskX < 0 || maskX >= maskW) continue;

                    maskMat.at<uint8_t>(y, x) =
                        maskData[maskY * maskW + maskX];
                }
            }
        }

        return {fullRGBA, maskMat};
    }

} // Split