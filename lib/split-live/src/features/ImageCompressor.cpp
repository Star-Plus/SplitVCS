//
// Created by lightum on 1/19/26.
//

#include "ImageCompressor.h"
#include <opencv2/opencv.hpp>

namespace Split {

    void ImageCompressor::encode(const std::string &source, const std::string &save, EncodeOptions options) {
        const cv::Mat src = cv::imread(source, cv::IMREAD_COLOR);
        if (src.empty()) {
            throw std::runtime_error("Failed to load image");
        }

        const bool success = cv::imwrite(save, src,
            {cv::IMWRITE_WEBP_QUALITY, options.quality}
            );

        if (!success) {
            throw std::runtime_error("Failed to save image");
        }
    }


}