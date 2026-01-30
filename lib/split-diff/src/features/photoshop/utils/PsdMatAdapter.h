//
// Created by Ahmed Mustafa on 1/29/2026.
//

#ifndef SPLITVCS_PSDMATADAPTER_H
#define SPLITVCS_PSDMATADAPTER_H

#include <opencv2/opencv.hpp>

#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdNativeFile.h"

namespace Split
{

    struct ExportedLayer
    {
        cv::Mat rgba;
        cv::Mat mask;
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

    };
} // Split

#endif //SPLITVCS_PSDMATADAPTER_H