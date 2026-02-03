//
// Created by Ahmed Mustafa on 1/31/2026.
//

#include "AssetBlockBuilder.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace Split
{
    void AssetBlockBuilder::combine(
        const std::string& building,
        std::set<BlockUnit>& blocks,
        const std::string& out
        )
    {
        std::ifstream inFile(building, std::ios::binary);
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
        size_t readOffset = 0;

        size_t shift = 0;

        for (const auto& block : blocks)
        {
            const size_t targetOffset = block.offset.offset + shift;
            const size_t blockSize = block.offset.length;

            if (currentOffset < targetOffset)
            {
                inFile.seekg(readOffset, std::ios::beg);

                size_t gap = targetOffset - currentOffset;
                while (gap > 0)
                {
                    const size_t chunk = std::min(BufferSize, gap);
                    inFile.read(buffer.data(), chunk);
                    outFile.write(buffer.data(), chunk);
                    gap -= chunk;

                    readOffset += chunk;
                }

                currentOffset = targetOffset;
            }

            const auto newBlockSize = block.offset.updatedLength;
            size_t remaining = newBlockSize;

            block.stream->clear();
            block.stream->seekg(0, std::ios::beg);

            while (remaining)
            {
                const size_t chunk = std::min(BufferSize, remaining);
                block.stream->read(buffer.data(), chunk);

                const std::streamsize readBytes = block.stream->gcount();

                if (readBytes <= 0)
                    throw std::runtime_error("Unexpected EOF while reading block stream");

                outFile.write(buffer.data(), chunk);
                currentOffset += chunk;
                remaining -= chunk;
            }

            shift += newBlockSize - blockSize;
        }

        if (readOffset < fileSize)
        {
            inFile.seekg(readOffset, std::ios::beg);
            size_t remaining = fileSize - readOffset;

            while (remaining > 0)
            {
                const size_t chunk = std::min(BufferSize, remaining);
                inFile.read(buffer.data(), chunk);
                outFile.write(buffer.data(), chunk);
                remaining -= chunk;
            }
        }

        inFile.close();
        outFile.close();
    }
} // Split