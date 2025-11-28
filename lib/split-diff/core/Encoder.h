//
// Created by Ahmed Mustafa on 9/21/2025.
//

#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <opencv2/opencv.hpp>
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
            Mat img = imread(oPath);
            Mat imgEdited = imread(ePath);

            if (img.empty() || imgEdited.empty()) {
                cerr << "Could not open images!\n";
                return;
            }

            if (img.size() != imgEdited.size() || img.type() != imgEdited.type()) {
                cerr << "Images must have the same size and type!\n";
                return;
            }

            // Convert to signed 16-bit
            Mat img16s, imgEdited16s;
            img.convertTo(img16s, CV_16S);
            imgEdited.convertTo(imgEdited16s, CV_16S);

            Mat diff = imgEdited16s - img16s;

            std::set<short> distinct = {};

            for (int i = 0; i < diff.rows; i++) {
                for (int j = 0; j < diff.cols; j++) {
                    // Print the pixel value
                    // std::cout << diff.at<short>(i, j) << " ";
                    distinct.insert(diff.at<short>(i, j));
                }
                // std::cout << std::endl;
            }

            std::cout << "Distinct: " << distinct.size() << std::endl;
        }

        static void decode(const std::string& oPath, const std::string& dPath, const std::string& outPath) {
            Mat img = imread(oPath);
            if (img.empty()) {
                cerr << "Could not open original image!\n";
                return;
            }

            // Convert to signed 16-bit
            Mat img16s;
            img.convertTo(img16s, CV_16S);

            ifstream ifs(dPath+".bin", ios::binary);
            if (!ifs) {
                cerr << "Failed to open diff file\n";
                return;
            }

            PixelDiff pd;
            while (ifs.read(reinterpret_cast<char*>(&pd), sizeof(PixelDiff))) {
                if (pd.x >= 0 && pd.x < img.cols && pd.y >= 0 && pd.y < img.rows) {
                    Vec3s& pixel = img16s.at<Vec3s>(pd.y, pd.x);
                    pixel[0] += pd.db;
                    pixel[1] += pd.dg;
                    pixel[2] += pd.dr;
                }
            }

            ifs.close();

            Mat imgDecoded;
            img16s.convertTo(imgDecoded, CV_8U);
            imwrite(outPath, imgDecoded);
            cout << "Decoded image saved to " << outPath << "\n";
        }
    };

}

#endif //ENCODER_H
