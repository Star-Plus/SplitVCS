//
// Created by Ahmed Mustafa on 9/21/2025.
//

#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

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

            ofstream ofs(dPath+".bin", ios::binary);
            if (!ofs) {
                cerr << "Failed to open output file\n";
                return;
            }

            // Iterate and store only nonzero diffs
            int count = 0;
            for (int y = 0; y < diff.rows; y++) {
                for (int x = 0; x < diff.cols; x++) {
                    Vec3s d = diff.at<Vec3s>(y, x); // signed 16-bit
                    if (d[0] || d[1] || d[2]) {
                        PixelDiff pd{(int16_t)x, (int16_t)y, d[0], d[1], d[2]};
                        ofs.write(reinterpret_cast<const char*>(&pd), sizeof(PixelDiff));
                        count++;
                    }
                }
            }

            ofs.close();
            cout << "Saved " << count << " changed pixels into diff.bin\n";
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
