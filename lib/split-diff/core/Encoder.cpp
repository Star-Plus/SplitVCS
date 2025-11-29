//
// Created by Ahmed Mustafa on 11/29/2025.
//

#include "Encoder.h"
#include <fstream>
#include <iostream>
#include <set>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace Split {

    void ImageEncoder::encode(const std::string& oPath, const std::string& ePath, const std::string& dPath) {
        Mat A = imread(oPath, IMREAD_COLOR);
        Mat B = imread(ePath, IMREAD_COLOR);

        if (A.empty() || B.empty()) {
            cerr << "Could not open images!\n";
            return;
        }

        if (A.size() != B.size()) {
            cerr << "Images must have the same size!\n";
            return;
        }

        if (A.type() != B.type()) {
            cerr << "Images must have the same type!\n";
            return;
        }

        Mat A16, B16;
        A.convertTo(A16, CV_16S);
        B.convertTo(B16, CV_16S);

        Mat diff;
        subtract(B16, A16, diff, noArray(), CV_16S);

        diff.convertTo(diff, CV_8SC3);

        Mat packed;
        diff.convertTo(packed, CV_8UC3, 1.0, 128.0);

        imwrite(dPath + "_packed.webp", packed, {IMWRITE_WEBP_QUALITY, 100});
    }

    void ImageEncoder::decode(const std::string& oPath, const std::string& dPath, const std::string& outPath) {
        Mat A = imread(oPath, IMREAD_COLOR);
        Mat packedDiff = imread(dPath + "_packed.webp", IMREAD_COLOR);

        if (A.empty() || packedDiff.empty()) {
            cerr << "Could not open images!\n";
            return;
        }

        if (A.size() != packedDiff.size()) {
            cerr << "Images must have the same size!\n";
            return;
        }

        Mat diff;
        packedDiff.convertTo(diff, CV_8S, 1.0, -128.0);

        Mat A16;
        A.convertTo(A16, CV_16S);

        Mat B16;
        add(A16, diff, B16, noArray(), CV_16S);

        Mat B8U;
        B16.convertTo(B8U, CV_8U);

        imwrite(outPath, B8U);
    }

}