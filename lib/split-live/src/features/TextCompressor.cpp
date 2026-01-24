//
// Created by Ahmed Mustafa on 1/23/2026.
//

#include "TextCompressor.h"
#include <fstream>

namespace Split
{
    std::string TextCompressor::encode(const std::string& src, const std::string& save, EncodeOptions options)
    {
        std::ifstream srcStream(src, std::ios::binary);
        std::ofstream saveStream(save, std::ios::binary);

        saveStream << srcStream.rdbuf();

        srcStream.close();
        saveStream.close();

        return save;
    }
}
