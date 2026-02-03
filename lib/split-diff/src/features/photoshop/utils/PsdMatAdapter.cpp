//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include "PsdMatAdapter.h"

#include "PsdSdk/PsdAllocator.h"
#include "PsdSdk/PsdChannelType.h"
#include "PsdSdk/PsdEndianConversion.h"
#include "PsdSdk/PsdLayerMask.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "PsdSdk/PsdDecompressRle.h"
#include "utils/adapters/OpenCvAdapter.h"
#include "utils/psd/ChannelUtils.h"
#include <iostream>

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

    std::vector<ImportedLayerChannel> PsdMatAdapter::matToPsdBuffer(
    const cv::Mat& mat,
    const std::vector<uint16_t>& compressionTypes,
    const std::vector<bool>& isAlphaChannels
)
{
    if (mat.empty())
        throw std::runtime_error("Image imported is empty");

    const auto planers = OpenCvAdapter::MatToPlanar(mat);
    const int width  = mat.cols;
    const int height = mat.rows;

    std::vector<ImportedLayerChannel> layerChannels;

    // Build channel list in PSD order
    std::vector<const std::vector<uint8_t>*> channelData;
    std::vector<std::vector<uint8_t>> synthesizedChannels;

    size_t colorIndex = 0;
    for (bool isAlpha : isAlphaChannels)
    {
        if (isAlpha)
        {
            if (!planers.a.empty())
            {
                channelData.push_back(&planers.a);
            }
            else
            {
                // Fallback opaque alpha
                synthesizedChannels.emplace_back(width * height, 255);
                channelData.push_back(&synthesizedChannels.back());
            }
        }
        else
        {
            if (colorIndex == 0) channelData.push_back(&planers.r);
            else if (colorIndex == 1) channelData.push_back(&planers.g);
            else if (colorIndex == 2) channelData.push_back(&planers.b);
            else
                throw std::runtime_error("Too many color channels");

            colorIndex++;
        }
    }

    if (channelData.size() != compressionTypes.size())
        throw std::runtime_error("Channel / compression count mismatch");

    // ------------------------------------------------------------
    // Write each channel
    // ------------------------------------------------------------
    for (size_t i = 0; i < channelData.size(); i++)
    {
        const auto& pixelData = *channelData[i];

        const uint16_t compression = compressionTypes[i];

        if (pixelData.size() != static_cast<size_t>(width * height))
            throw std::runtime_error("Channel size mismatch");

        auto stream = std::make_shared<std::stringstream>(
            std::ios::binary | std::ios::in | std::ios::out
        );

        uint16_t compressionBE =
            psd::endianUtil::NativeToBigEndian(compression);

        size_t actualSize = 0;

        // --------------------------------------------------------
        // RAW
        // --------------------------------------------------------
        if (compression == psd::compressionType::RAW)
        {
            stream->write(reinterpret_cast<char*>(&compressionBE), 2);
            stream->write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

            actualSize = 2 + pixelData.size();
        }
        // --------------------------------------------------------
        // RLE (PSD CORRECT)
        // --------------------------------------------------------
        else if (compression == psd::compressionType::RLE)
        {
            stream->write(reinterpret_cast<char*>(&compressionBE), 2);

            // Reserve row byte counts
            std::vector<uint16_t> rowSizes(height);
            const std::streampos rowSizePos = stream->tellp();

            for (int y = 0; y < height; y++)
            {
                uint16_t zero = 0;
                stream->write(reinterpret_cast<char*>(&zero), 2);
            }

            size_t rleDataSize = 0;

            // Compress each row independently
            for (int y = 0; y < height; y++)
            {
                const uint8_t* rowPtr =
                    pixelData.data() + (y * width);

                std::vector<uint8_t> compressedRow(width * 2);

                const unsigned int rowSize =
                    psd::imageUtil::CompressRle(
                        rowPtr,
                        compressedRow.data(),
                        width
                    );

                if (rowSize > 0xFFFF)
                    throw std::runtime_error("RLE row too large");

                rowSizes[y] = static_cast<uint16_t>(rowSize);
                stream->write(reinterpret_cast<char*>(compressedRow.data()), rowSize);

                rleDataSize += rowSize;
            }

            // Write row sizes (big-endian)
            const std::streampos endPos = stream->tellp();
            stream->seekp(rowSizePos);

            for (int y = 0; y < height; y++)
            {
                uint16_t sizeBE =
                    psd::endianUtil::NativeToBigEndian(rowSizes[y]);
                stream->write(reinterpret_cast<char*>(&sizeBE), 2);
            }

            stream->seekp(endPos);

            actualSize = 2 + (height * 2) + rleDataSize;
        }
        // --------------------------------------------------------
        // ZIP → fallback RAW (still valid PSD)
        // --------------------------------------------------------
        else if (compression == psd::compressionType::ZIP ||
                 compression == psd::compressionType::ZIP_WITH_PREDICTION)
        {
            stream->write(reinterpret_cast<char*>(&compressionBE), 2);
            stream->write(reinterpret_cast<const char*>(pixelData.data()),
                          pixelData.size());

            actualSize = 2 + pixelData.size();
        }
        else
        {
            throw std::runtime_error(
                "Unsupported compression type: " +
                std::to_string(compression)
            );
        }

        stream->seekg(0, std::ios::beg);
        layerChannels.push_back({stream, actualSize});
    }

    return layerChannels;
}
} // Split