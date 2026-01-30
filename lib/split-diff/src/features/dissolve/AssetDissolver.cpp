//
// Created by Ahmed Mustafa on 1/31/2026.
//

#include "AssetDissolver.h"
#include <fstream>
#include <vector>

namespace Split
{
    void AssetDissolver::dissolve(const std::string& base, const std::string& out, std::set<OffsetBound>& excludes)
    {
        std::ifstream inFile(base, std::ios::binary);
        std::ofstream outFile(out, std::ios::out | std::ios::binary);

        if (!inFile || !outFile) throw std::runtime_error("Failed to open file");

        inFile.seekg(0, std::ios::end);
        size_t fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        constexpr size_t BufferSize = 64 * 1024;
        std::vector<char> buffer(BufferSize);

        size_t currentOffset = 0;

        for (const auto& [offset, length] : excludes)
        {
            if (offset >= fileSize) continue;

            const size_t copyEnd = std::min(offset, fileSize);

            while (currentOffset < copyEnd)
            {
                const size_t toRead = std::min(BufferSize, copyEnd - currentOffset);
                inFile.read(buffer.data(), toRead);
                outFile.write(buffer.data(), toRead);
                currentOffset += toRead;
            }

            size_t skipEnd = std::min(offset + length , fileSize);
            inFile.seekg(skipEnd, std::ios::beg);
            currentOffset = skipEnd;
        }

        while (currentOffset < fileSize)
        {
            const size_t toRead = std::min(BufferSize, fileSize - currentOffset);
            inFile.read(buffer.data(), toRead);
            outFile.write(buffer.data(), toRead);
            currentOffset += toRead;
        }
    }
} // Split