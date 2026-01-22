//
// Created by Ahmed Mustafa on 11/29/2025.
//

#include "ImageEncoder.h"
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "debug/ImageDebugUtils.cpp"
#include "utils/PixelCalculator.h"
#include "utils/ReadAdapter.h"

using namespace cv;
using namespace std;

namespace Split {

    ImageEncoder::ImageEncoder() : logger(true) {}

    void ImageEncoder::encode(const std::istream& v1, const std::istream& v2, std::ostream& output) {
        const Mat A = ReadAdapter::fastReadImage(const_cast<std::istream&>(v1), IMREAD_COLOR);
        const Mat B = ReadAdapter::fastReadImage(const_cast<std::istream&>(v2), IMREAD_COLOR);

        if (A.empty() || B.empty()) {
            logger.error("Could not open images!");
            return;
        }

        if (A.size() != B.size()) {
            logger.error("Images must have the same size!");
            return;
        }

        if (A.type() != B.type()) {
            logger.error("Images must have the same type!");
            return;
        }

        Mat packedDiff(A.size(), A.type());

        PixelCalculator::clockDifference(A, B, packedDiff);

        const auto distinctValuesCount = ImageDebugUtils::calculateDistinctValuesCount(packedDiff);
        logger.debug("Number of distinct values: " + to_string(distinctValuesCount));

        std::vector<uchar> compressed;
        imencode(".webp", packedDiff, compressed, {IMWRITE_WEBP_QUALITY, 101});

        if (compressed.empty()) {
            logger.error("Could not encode difference image!");
            return;
        }

        if (output.good()){
            output.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
        } else {
            logger.error("Could not write to output stream!");
        }

    }

}