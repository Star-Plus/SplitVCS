//
// Created by Ahmed Mustafa on 9/21/2025.
//

#ifndef ENCODER_H
#define ENCODER_H

#include <filesystem>
#include "main/Logger.h"
#include <string>
#include "interfaces/IEncoder.h"

namespace Split {

    struct PixelDiff {
        int16_t x, y;
        int16_t db, dg, dr;
    };

    class ImageEncoder final : public Encoder {

    public:
        ImageEncoder();

        std::string encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out) override;
        void encode(const std::istream& v1, const std::istream& v2, std::ostream& output) override;

    private:
        Logger logger;
    };

}

#endif //ENCODER_H
