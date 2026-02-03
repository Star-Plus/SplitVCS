//
// Created by Ahmed Mustafa on 1/31/2026.
//

#ifndef SPLITVCS_OPENCVADAPTER_H
#define SPLITVCS_OPENCVADAPTER_H

#include <vector>

#include "opencv2/core/mat.hpp"

namespace Split::OpenCvAdapter
{

    struct PlanarChannels
    {
        std::vector<uint8_t> r;
        std::vector<uint8_t> g;
        std::vector<uint8_t> b;
        std::vector<uint8_t> a;
    };

    inline PlanarChannels MatToPlanar(const cv::Mat& img)
    {
        if (img.type() != CV_8UC3 && img.type() != CV_8UC4)
            throw std::runtime_error("Only 8-bit BGR/BGRA images supported");

        const int width  = img.cols;
        const int height = img.rows;
        const size_t pixelCount = static_cast<size_t>(width) * height;

        PlanarChannels out;
        out.r.resize(pixelCount);
        out.g.resize(pixelCount);
        out.b.resize(pixelCount);

        const bool hasAlpha = img.channels() == 4;
        if (hasAlpha)
            out.a.resize(pixelCount);

        size_t idx = 0;
        for (int y = 0; y < height; ++y)
        {
            if (hasAlpha)
            {
                const cv::Vec4b* row = img.ptr<cv::Vec4b>(y);
                for (int x = 0; x < width; ++x, ++idx)
                {
                    out.b[idx] = row[x][0];
                    out.g[idx] = row[x][1];
                    out.r[idx] = row[x][2];
                    out.a[idx] = row[x][3];
                }
            }
            else
            {
                const cv::Vec3b* row = img.ptr<cv::Vec3b>(y);
                for (int x = 0; x < width; ++x, ++idx)
                {
                    out.b[idx] = row[x][0];
                    out.g[idx] = row[x][1];
                    out.r[idx] = row[x][2];
                }
            }
        }

        return out;
    }
}

#endif //SPLITVCS_OPENCVADAPTER_H