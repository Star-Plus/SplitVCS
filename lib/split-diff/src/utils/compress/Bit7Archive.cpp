//
// Created by Ahmed Mustafa on 2/2/2026.
//

#include "Bit7Archive.h"

#include "bit7z/bitfilecompressor.hpp"

namespace fs = std::filesystem;

namespace Split
{
    Bit7Archive::Bit7Archive() :
    lib("7z.dll"),
    compressor(lib, bit7z::BitFormat::SevenZip),
    extractor(lib, bit7z::BitFormat::SevenZip)
    {
    }

    void Bit7Archive::SaveDirToArchive(const std::string& dirPath, const std::string& savePath) const
    {
        std::map<std::string, std::string> filesToCompress;

        for (const auto& entry : fs::recursive_directory_iterator(dirPath))
        {
            if (entry.is_regular_file())
            {
                std::string absolutePath = entry.path().string();

                const std::string relativePath = fs::relative(entry.path(), dirPath).string();

                filesToCompress[absolutePath] = relativePath;
            }
        }

        // Use the generic compress method with the map
        compressor.compress(filesToCompress, savePath);
    }

    void Bit7Archive::ExtractArchive(const std::string& archive, const std::string& savePath) const
    {
        extractor.extract(archive, savePath);
    }
} // Split