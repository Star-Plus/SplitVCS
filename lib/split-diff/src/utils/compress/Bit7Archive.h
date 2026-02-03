//
// Created by Ahmed Mustafa on 2/2/2026.
//

#ifndef SPLITVCS_BIT7ARCHIVE_H
#define SPLITVCS_BIT7ARCHIVE_H

#include <string>

#include "bit7z/bitfileextractor.hpp"
#include "bit7z/bit7zlibrary.hpp"
#include "bit7z/bitfilecompressor.hpp"

namespace Split
{
    class Bit7Archive
    {
    public:
        Bit7Archive();

        void SaveDirToArchive(const std::string& dirPath, const std::string& savePath) const;
        void ExtractArchive(const std::string& archive, const std::string& savePath) const;
    private:

        bit7z::Bit7zLibrary lib;
        bit7z::BitFileCompressor compressor;
        bit7z::BitFileExtractor extractor;
    };
} // Split

#endif //SPLITVCS_BIT7ARCHIVE_H