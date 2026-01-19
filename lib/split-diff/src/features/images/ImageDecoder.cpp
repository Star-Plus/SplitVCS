#include "ImageDecoder.h"
#include <fstream>
#include <opencv2/opencv.hpp>

#include "utils/FormatSniffer.h"
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
                    pDiff[i] = uchar(int(pB[i]) + int(pA[i]) - 128);
                }
            }
        }

    };

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

        const RowConstruct constructedKernel(&A, &B, &constructed);
        parallel_for_(Range(0, A.rows), constructedKernel);

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