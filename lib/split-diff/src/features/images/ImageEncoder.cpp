//
// Created by Ahmed Mustafa on 11/29/2025.
//

#include "ImageEncoder.h"
#include <opencv2/opencv.hpp>

#include "ImageDecoder.h"
#include "debug/ImageDebugUtils.cpp"
#include "utils/PixelCalculator.h"
#include "utils/ReadAdapter.h"

using namespace cv;
using namespace std;

namespace Split {

    ImageEncoder::ImageEncoder() : logger(true, "Image Encoder") {}

    std::string ImageEncoder::encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out)
    {
        logger.setCheckPoint();

        logger.debug("Decode v1");
        const Mat v1Mat = ImageDecoder::decode(base, deltas);
        logger.debug("Decoded");

        logger.debug("Encode");

        const Mat v2Mat = imread(v2, IMREAD_COLOR);

        Mat outMat(v1Mat.size(), v1Mat.type());

        PixelCalculator::clockDifference(v1Mat, v2Mat, outMat);
        imwrite(out+".webp", outMat, {IMWRITE_WEBP_QUALITY, 101});

        logger.debug("Encoded");

        return out + ".webp";
    }

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