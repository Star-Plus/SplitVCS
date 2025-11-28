//
// Created by Ahmed Mustafa on 9/21/2025.
//

#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <zstd.h>
#include <fstream>
#include <iostream>
#include <set>

using namespace cv;
using namespace std;

namespace Split {

    struct PixelDiff {
        int16_t x, y;
        int16_t db, dg, dr;
    };

    class ImageEncoder {

    public:
        static void encode(const std::string& oPath, const std::string& ePath, const std::string& dPath) {
            Mat A = imread(oPath, IMREAD_COLOR);
            Mat B = imread(ePath, IMREAD_COLOR);

            cout << "A.type = " << A.type() << "  channels = " << A.channels() << endl;
            cout << "B.type = " << B.type() << "  channels = " << B.channels() << endl;


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

            std::set<std::tuple<int, int, int>> distinctValues;
            for (int y=0; y < diff.rows; y++) {
                for (int x = 0; x < diff.cols; x++) {
                    const auto v = diff.at<Vec<char, 3>>(y, x);
                    distinctValues.insert(std::make_tuple(v[0], v[1], v[2]));
                }
            }

            Mat packed;
            diff.convertTo(packed, CV_8UC3, 1.0, 128.0);

            std::cout << "Distinct difference values count: " << distinctValues.size() << "\n";

            imwrite(dPath + "_packed.webp", packed, {IMWRITE_WEBP_QUALITY, 100});

            if (!diff.isContinuous()) {
                throw runtime_error("Diff matrix is not continuous in memory!");
            }

            size_t srcSize = diff.total() * diff.elemSize();
            void* srcBuffer = diff.data;

            size_t destCapacity = ZSTD_compressBound(srcSize);
            std::vector<char> compressedBuffer(destCapacity);

            size_t compressedSize = ZSTD_compress(compressedBuffer.data(), destCapacity, srcBuffer, srcSize, 3);
            if (ZSTD_isError(compressedSize)) {
                throw runtime_error("Compression failed: " + std::string(ZSTD_getErrorName(compressedSize)));
            }
            std::ofstream outFile(dPath + "_diff.zst", std::ios::binary);
            outFile.write(compressedBuffer.data(), compressedSize);
        }

        static void decode(const std::string& oPath, const std::string& dPath, const std::string& outPath) {
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
    };

}

#endif //ENCODER_H
