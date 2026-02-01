//
// Created by Ahmed Mustafa on 1/31/2026.
//

#include "AssetDissolver.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace Split
{
    void AssetDissolver::dissolve(const std::string& base, const std::string& out, std::set<OffsetBound>& excludes)
    {
        std::ifstream inFile(base, std::ios::binary);
        std::ofstream outFile(out, std::ios::out | std::ios::binary);

        if (!inFile || !outFile) throw std::runtime_error("Failed to open file");

        inFile.seekg(0, std::ios::end);
        const size_t fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        constexpr size_t BufferSize = 64 * 1024;
        std::vector<char> buffer(BufferSize);

        size_t currentOffset = 0;

        for (const auto& exclude : excludes)
        {
            if (exclude.offset >= fileSize) continue;

            while (currentOffset < exclude.offset)
            {
                const size_t toRead = std::min(BufferSize, exclude.offset - currentOffset);
                inFile.read(buffer.data(), toRead);
                outFile.write(buffer.data(), toRead);
                currentOffset += toRead;
            }

            size_t skipEnd = std::min(exclude.offset + exclude.length , fileSize);
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

    void AssetDissolver::slice(
        const std::string& base,
        const OffsetBound& offsetBound,
        const std::string& out,
        const std::ios::openmode mode
        )
    {
        std::ifstream in(base, std::ios::binary);
        if (!in)
            throw std::runtime_error("Failed to open base file: " + base);

        std::ofstream outFile(out, std::ios::binary | mode);
        if (!outFile)
            throw std::runtime_error("Failed to open output file: " + out);

        // Seek to start offset
        in.seekg(static_cast<std::streamoff>(offsetBound.offset), std::ios::beg);
        if (!in)
            throw std::runtime_error("Failed to seek to offset");

        constexpr size_t bufferSize = 64 * 1024; // 64 KB
        std::vector<char> buffer(bufferSize);

        size_t remaining = offsetBound.length;

        while (remaining > 0)
        {
            const size_t toRead = std::min(bufferSize, remaining);
            in.read(buffer.data(), static_cast<std::streamsize>(toRead));

            const std::streamsize readBytes = in.gcount();
            if (readBytes <= 0)
                throw std::runtime_error("Unexpected EOF while reading base file");

            outFile.write(buffer.data(), readBytes);
            if (!outFile)
                throw std::runtime_error("Failed while writing output file");

            remaining -= static_cast<size_t>(readBytes);
        }
    }
} // Split