#include "ImageDecoder.h"
#include <fstream>
#include <opencv2/opencv.hpp>

#include "utils/FormatSniffer.h"
#include "utils/PixelCalculator.h"
#include "utils/ReadAdapter.h"

using namespace cv;
using namespace std;

namespace Split {

    void ImageDecoder::decode(const std::istream& base, const std::istream& delta, std::ostream& out) {

        ImageFormat originalFormat = FormatSniffer::sniff(const_cast<std::istream&>(base));

        const Mat A = ReadAdapter::fastReadImage(const_cast<std::istream&>(base), IMREAD_COLOR);
        const Mat B = ReadAdapter::fastReadImage(const_cast<std::istream&>(delta), IMREAD_COLOR);

        if (A.empty() || B.empty()) {
            logger.error("Could not open images!");
            return;
        }

        if (A.size() != B.size()) {
            logger.error("Images must have the same size!");
            return;
        }

        Mat constructed(A.size(), A.type());

        PixelCalculator::clockAdder(A, B, constructed);

        std::vector<uchar> uncompressed;

        try {
            imencode(originalFormat.extension, constructed, uncompressed, originalFormat.params);
        } catch (const Exception& e) {
            logger.error(e.what());
            return;
        }

        if (uncompressed.empty()) {
            logger.error("Could not encode difference image!");
            return;
        }

        if (out.good()){
            out.write(reinterpret_cast<const char*>(uncompressed.data()), uncompressed.size());
        } else {
            logger.error("Could not write to output stream!");
        }
    };

}