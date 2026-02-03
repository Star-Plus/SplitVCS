//
// Created by Ahmed Mustafa on 1/31/2026.
//

#include "PsdChannelSizeUpdater.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "PsdSdk/PsdEndianConversion.h"

namespace Split
{
    void PsdChannelSizeUpdater::updateChannelSizes(
        const std::string& psdFilePath,
        const std::vector<size_t>& channelSizeFieldOffsets,
        const std::vector<uint32_t>& newSizes,
        const size_t layerMaskSectionSizeOffset,
        const size_t layerMaskSectionNewSize

    )
    {
        if (channelSizeFieldOffsets.size() != newSizes.size())
        {
            throw std::runtime_error(
                "Mismatch between channel count and size count: " +
                std::to_string(channelSizeFieldOffsets.size()) + " offsets vs " +
                std::to_string(newSizes.size()) + " sizes"
            );
        }


        std::fstream psdFile(psdFilePath, std::ios::binary | std::ios::in | std::ios::out);
        if (!psdFile.is_open())
        {
            throw std::runtime_error("Failed to open PSD file for modification: " + psdFilePath);
        }

        const uint32_t newSizeBe = psd::endianUtil::NativeToBigEndian(layerMaskSectionNewSize);
        psdFile.seekp(layerMaskSectionSizeOffset, std::ios::beg);
        psdFile.write(reinterpret_cast<const char*>(&newSizeBe), sizeof(uint32_t));

        if (!psdFile.good())
        {
            throw std::runtime_error("Failed to write new layer mask section size");
        }

        for (size_t i = 0; i < channelSizeFieldOffsets.size(); i++)
        {
            const size_t offset = channelSizeFieldOffsets[i];
            const uint32_t newSize = newSizes[i];

            const uint32_t sizeBE = psd::endianUtil::NativeToBigEndian(newSize);

            psdFile.seekp(offset, std::ios::beg);
            if (!psdFile.good())
            {
                throw std::runtime_error(
                    "Failed to seek to channel size field at offset " + 
                    std::to_string(offset)
                );
            }

            psdFile.write(reinterpret_cast<const char*>(&sizeBE), sizeof(uint32_t));
            if (!psdFile.good())
            {
                throw std::runtime_error(
                    "Failed to write new channel size at offset " + 
                    std::to_string(offset)
                );
            }
        }

        psdFile.flush();
        psdFile.close();        
    }
}
