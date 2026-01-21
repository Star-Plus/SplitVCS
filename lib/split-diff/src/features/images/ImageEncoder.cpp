//
// Created by Ahmed Mustafa on 11/29/2025.
//

#include "ImageEncoder.h"
#include <fstream>
#include <opencv2/opencv.hpp>
#include "debug/ImageDebugUtils.cpp"
#include "utils/ReadAdapter.h"

using namespace cv;
using namespace std;

namespace Split {

    class RowConstruct final : public ParallelLoopBody {
        const Mat *A;
        const Mat *B;
        Mat* Construct;
    public:
        RowConstruct(
            const Mat* _A,
            const Mat* _B,
            Mat* _Diff
        ) : A(_A), B(_B), Construct(_Diff) {}

        void operator()(const Range& range) const override {
            for (int r = range.start; r < range.end; ++r) {
                const uchar* pA = A->ptr<uchar>(r);
                const uchar* pB = B->ptr<uchar>(r);
                uchar* pDiff = Construct->ptr<uchar>(r);

                const int len = A->cols * A->channels();

                for (int i = 0; i < len; ++i) {
                    pDiff[i] = uchar(int(pB[i]) - int(pA[i]) + 128);
                }
            }
        }

    };

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

        const RowConstruct diffKernel(&A, &B, &packedDiff);
        parallel_for_(Range(0, A.rows), diffKernel);

        const auto distinctValuesCount = ImageDebugUtils::calculateDistinctValuesCount(packedDiff);
        logger.debug("Number of distinct values: " + to_string(distinctValuesCount));

        std::vector<uchar> compressed;
        imencode(".webp", packedDiff, compressed, {IMWRITE_WEBP_QUALITY, 200});

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