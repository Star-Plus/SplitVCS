//
// Created by Ahmed Mustafa on 1/29/2026.
//

#ifndef SPLITVCS_PSDMATADAPTER_H
#define SPLITVCS_PSDMATADAPTER_H

#include <opencv2/opencv.hpp>

#include "PsdSdk/PsdCompressionType.h"
#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdExportChannel.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdNativeFile.h"

namespace Split
{

    struct ExportedLayer
    {
        cv::Mat rgba;
        cv::Mat mask;
    };

    struct ImportedLayerChannel
    {
        std::shared_ptr<std::istream> stream;
        size_t size;
    };

    class PsdMatAdapter
    {
    public:
        PsdMatAdapter() = default;

        ExportedLayer pixelsToMat(
            const psd::Document* document,
            psd::NativeFile& file,
            psd::Allocator& allocator,
            psd::Layer* layer
        );

        std::vector<ImportedLayerChannel> matToPsdBuffer(
            const cv::Mat& mat,
            const std::vector<uint16_t>& compressionTypes,
            const std::vector<bool>& isAlphaChannels
        );

    };
} // Split

#endif //SPLITVCS_PSDMATADAPTER_H