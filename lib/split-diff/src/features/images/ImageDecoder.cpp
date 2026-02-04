#include "ImageDecoder.h"
#include <fstream>

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
    }

    void ImageDecoder::decode(const std::string& base, std::stack<std::string>& deltas, std::string& out)
    {
        std::fstream stream(base, std::ios::in);
        const auto [extension, params] = FormatSniffer::sniff(stream);
        const Mat OutMat = decode(base, deltas);
        imwrite(out, OutMat, params);
    }

    Mat ImageDecoder::decode(const std::string& base, std::stack<std::string>& deltas)
    {
        const Mat BaseMat = imread(base + ".webp", IMREAD_COLOR);

        if (BaseMat.empty())
        {
            throw std::runtime_error("Could not base image!");
        }

        if (deltas.empty())
        {
            return BaseMat;
        }

        Mat OutMat(BaseMat.size(), BaseMat.type());
        Mat MiddleOutMat(BaseMat.size(), BaseMat.type());
        BaseMat.copyTo(MiddleOutMat);

        while (!deltas.empty())
        {
            const Mat DeltaMat = imread(deltas.top() + ".webp", IMREAD_COLOR);
            deltas.pop();

            PixelCalculator::clockAdder(MiddleOutMat, DeltaMat, OutMat);

            if (!deltas.empty())
                OutMat.copyTo(MiddleOutMat);
        }

        return OutMat;
    }
}
