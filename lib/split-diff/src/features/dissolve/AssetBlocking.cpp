//
// Created by Ahmed Mustafa on 1/31/2026.
//

#include "AssetBlocking.h"
#include <fstream>
#include <vector>

namespace Split
{
    void AssetBlocking::combine(const std::string& building, std::set<BlockUnit>& blocks, const std::string& out)
    {
        std::ifstream inFile(building, std::ifstream::in);
        if (!inFile.is_open()) throw std::runtime_error("Cannot open file");

        std::ofstream outFile(out, std::ios::binary | std::ios::trunc);
        if (!outFile)
            throw std::runtime_error("Cannot open output file");

        inFile.seekg(0, std::ios::end);
        const size_t fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        constexpr size_t BufferSize = 64 * 1024;
        std::vector<char> buffer(BufferSize, 0);

        size_t currentOffset = 0;

        for (const auto& block : blocks)
        {
            const size_t targetOffset = block.offset.offset;
            const size_t blockSize = block.offset.length;

            if (currentOffset < targetOffset)
            {
                inFile.seekg(currentOffset, std::ios::beg);

                size_t gap = targetOffset - currentOffset;
                while (gap > 0)
                {
                    const size_t chunk = std::min(BufferSize, gap);
                    inFile.read(buffer.data(), chunk);
                    outFile.write(buffer.data(), chunk);
                    gap -= chunk;
                }
                currentOffset = targetOffset;
            }

            size_t remaining = blockSize;
            while (remaining > 0)
            {
                const size_t chunk = std::min(BufferSize, remaining);
                block.stream.read(buffer.data(), chunk);

                if (block.stream.gcount() != static_cast<std::streamsize>(chunk))
                    throw std::runtime_error("Cannot read from stream");

                outFile.write(buffer.data(), chunk);
                remaining -= chunk;
                currentOffset += chunk;
            }
        }

        if (currentOffset < fileSize)
        {
            inFile.seekg(currentOffset, std::ios::beg);
            size_t remaining = fileSize - currentOffset;
            while (remaining > 0)
            {
                const size_t chunk = std::min(BufferSize, remaining);
                inFile.read(buffer.data(), chunk);
                outFile.write(buffer.data(), chunk);
                remaining -= chunk;
            }
        }
    }
} // Split