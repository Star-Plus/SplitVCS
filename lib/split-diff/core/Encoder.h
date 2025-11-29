//
// Created by Ahmed Mustafa on 9/21/2025.
//

#ifndef ENCODER_H
#define ENCODER_H

#include <filesystem>
#include <string>

namespace Split {

    struct PixelDiff {
        int16_t x, y;
        int16_t db, dg, dr;
    };

    class ImageEncoder {

    public:
        static void encode(const std::string& oPath, const std::string& ePath, const std::string& dPath);

        static void decode(const std::string& oPath, const std::string& dPath, const std::string& outPath);
    };

}

#endif //ENCODER_H
