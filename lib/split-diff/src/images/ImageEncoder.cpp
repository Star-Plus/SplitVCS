//
// Created by Ahmed Mustafa on 11/29/2025.
//

#include "ImageEncoder.h"
#include <fstream>
#include <iostream>
#include <set>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace Split {

    cv::Mat fastReadImage(std::istream& is, int flags) {
        is.seekg(0, ios::end);
        const std::streamsize sz = is.tellg();
        is.seekg(0, ios::beg);

        if (sz <= 0) {
            return Mat();
        }

        std::vector<char> buffer(static_cast<size_t>(sz));
        is.read(buffer.data(), sz);
        return imdecode(Mat(buffer), flags);
    }

    class RowDiff : public cv::ParallelLoopBody {
    private:
        const Mat *A;
        const Mat *B;
        Mat* Diff;
    public:
        RowDiff(
            const Mat* _A,
            const Mat* _B,
            Mat* _Diff
        ) : A(_A), B(_B), Diff(_Diff) {}

        void operator()(const cv::Range& range) const override {
            for (int i = range.start; i < range.end; ++i) {
                const uchar* pA = A->ptr<uchar>(i);
                const uchar* pB = B->ptr<uchar>(i);
                uchar* pDiff = Diff->ptr<uchar>(i);
                
                int len = A->cols * A->channels();

                for (int i = 0; i < len; ++i) {
                    int val = static_cast<int>(pB[i]) - static_cast<int>(pA[i]) + 128;
                    pDiff[i] = cv::saturate_cast<uchar>(val);
                }
            }
        }

    };

    ImageEncoder::ImageEncoder() : logger(true) {}

    void ImageEncoder::encode(const std::istream& v1, const std::istream& v2, std::ostream& output) {
        Mat A = fastReadImage(const_cast<std::istream&>(v1), IMREAD_COLOR);
        Mat B = fastReadImage(const_cast<std::istream&>(v2), IMREAD_COLOR);

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

        RowDiff diffKernel(&A, &B, &packedDiff);
        parallel_for_(Range(0, A.rows), diffKernel);

        logger.info("Difference image size: " + to_string(packedDiff.total() * packedDiff.elemSize()) + " bytes");

        std::vector<uchar> buffer;

        imencode(".png", packedDiff, buffer, {IMWRITE_PNG_COMPRESSION, 9});

        if (buffer.empty()) {
            logger.error("Could not encode difference image!");
            return;
        }

        if (output.good()){
            output.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        } else {
            logger.error("Could not write to output stream!");
        }

    }

}