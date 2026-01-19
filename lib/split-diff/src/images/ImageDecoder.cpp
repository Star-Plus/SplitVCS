#include "ImageDecoder.h"
#include <fstream>
#include <iostream>
#include <set>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace Split {

    void ImageDecoder::decode(const std::istream& base, const std::istream& delta, std::ostream& out) {
        // Mat A = imread(oPath, IMREAD_COLOR);
        // Mat packedDiff = imread(dPath + "_packed.webp", IMREAD_COLOR);

        // if (A.empty() || packedDiff.empty()) {
        //     cerr << "Could not open images!\n";
        //     return;
        // }

        // if (A.size() != packedDiff.size()) {
        //     cerr << "Images must have the same size!\n";
        //     return;
        // }

        // Mat diff;
        // packedDiff.convertTo(diff, CV_8S, 1.0, -128.0);

        // Mat A16;
        // A.convertTo(A16, CV_16S);

        // Mat B16;
        // add(A16, diff, B16, noArray(), CV_16S);

        // Mat B8U;
        // B16.convertTo(B8U, CV_8U);

        // imwrite(outPath, B8U);
    }

}