//
// Created by lightum on 1/19/26.
//

#ifndef SPLITVCS_FORMATSNIFFER_H
#define SPLITVCS_FORMATSNIFFER_H

#include <string>
#include <vector>
#include <istream>
#include <opencv2/opencv.hpp>

namespace Split {

    struct ImageFormat {
        std::string extension;
        std::vector<int> params;
    };

    class FormatSniffer {
    public:
        static ImageFormat sniff(std::istream& stream) {
            const auto pos = stream.tellg();

            unsigned char sig[12];
            stream.read(reinterpret_cast<char*>(sig), 12);

            stream.seekg(pos);

            if (sig[0] == 0x89 && sig[1] == 0x50 && sig[2] == 0x4E && sig[3] == 0x47) {
                return {".png", {cv::IMWRITE_PNG_COMPRESSION, 3}};
            }

            if (sig[0] == 0xFF && sig[1] == 0xD8 && sig[2] == 0xFF) {
                return {".png", {cv::IMWRITE_JPEG_QUALITY, 100}};
            }

            if (sig[0] == 'R' && sig[1] == 'I' && sig[2] == 'F' && sig[3] == 'F' &&
                sig[8] == 'W' && sig[9] == 'E' && sig[10] == 'B' && sig[11] == 'P') {

                return { ".webp", { cv::IMWRITE_WEBP_QUALITY, 101 } };
            }

            if (sig[0] == 'B' && sig[1] == 'M') {
                return { ".bmp", {} };
            }

            return {".png", {}};
        }
    };

}
    #endif //SPLITVCS_FORMATSNIFFER_H