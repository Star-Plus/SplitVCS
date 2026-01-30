//
// Created by Ahmed Mustafa on 1/30/2026.
//

#include <filesystem>
#include "PsdDecoder.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/mat.hpp"
#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdLayerMaskSection.h"
#include "PsdSdk/PsdMallocAllocator.h"
#include "PsdSdk/PsdNativeFile.h"
#include "PsdSdk/PsdParseDocument.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "PsdSdk/PsdLayer.h"

namespace psd
{
    struct ExportDocument;
    struct LayerMaskSection;
    struct Document;
}

namespace Split
{
    void PsdDecoder::decode(const std::string& base, std::stack<std::string>& deltas, std::string& out)
    {
        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);

        std::filesystem::path basePath(base + ".psd");
        if (!file.OpenRead(basePath.wstring().c_str())) throw std::runtime_error("Could not open base file");

        psd::Document* document = psd::CreateDocument(&file, &allocator);
        psd::LayerMaskSection* layerMask = psd::ParseLayerMaskSection(document, &file, &allocator);

        for (unsigned int i = 0; i < layerMask->layerCount; i++)
        {
            psd::Layer* layer = &layerMask->layers[i];
            const std::string layerName = layer->name.c_str();
            const std::string layerPath = base + "-" + layerName + ".webp";

            cv::Mat image = cv::imread(layerPath, cv::IMREAD_COLOR);
            if (image.empty()) throw std::runtime_error("Could not open image");

            if (image.channels() == 3) cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);

            const int w = image.cols;
            const int h = image.rows;
            const size_t planeSize = static_cast<size_t>(w * h);

            uint8_t* r = static_cast<uint8_t*>(allocator.Allocate(planeSize, 16u));
            uint8_t* g = static_cast<uint8_t*>(allocator.Allocate(planeSize, 16u));
            uint8_t* b = static_cast<uint8_t*>(allocator.Allocate(planeSize, 16u));
            uint8_t* a = static_cast<uint8_t*>(allocator.Allocate(planeSize, 16u));

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    auto pixel = image.at<cv::Vec4b>(y, x);
                    size_t offset = static_cast<size_t>(y) * w + x;
                    b[offset] = pixel[0];
                    g[offset] = pixel[1];
                    r[offset] = pixel[2];
                    a[offset] = pixel[3];
                }
            }

            allocator.Free(r); allocator.Free(g); allocator.Free(b); allocator.Free(a);

        }

        psd::DestroyLayerMaskSection(layerMask, &allocator);
        psd::DestroyDocument(document, &allocator);
    }
} // Split