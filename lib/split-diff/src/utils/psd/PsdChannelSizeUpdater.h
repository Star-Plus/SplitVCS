//
// Created by Ahmed Mustafa on 1/31/2026.
//

#ifndef SPLITVCS_PSDCHANNELSIZEUPDATER_H
#define SPLITVCS_PSDCHANNELSIZEUPDATER_H

#include <string>
#include <vector>
#include <cstdint>

namespace Split
{
    /**
     * @brief Utility class to update channel size fields in a PSD file
     * 
     * The PSD file format stores each channel's size as a uint32_t (big-endian)
     * in the layer record. When we re-encode channels, their sizes may change,
     * so we need to update these size fields before combining the file.
     */
    class PsdChannelSizeUpdater
    {
    public:
        /**
         * @brief Updates channel size fields in a PSD file
         * 
         * @param psdFilePath Path to the PSD file to modify
         * @param channelSizeFieldOffsets File offsets where each channel's size field is located
         * @param newSizes New size values to write (including compression header)
         * 
         * This function:
         * 1. Opens the PSD file for in-place modification
         * 2. For each channel, seeks to its size field offset
         * 3. Writes the new size as a big-endian uint32_t
         * 4. Ensures file integrity by flushing changes
         */
        static void updateChannelSizes(
            const std::string& psdFilePath,
            const std::vector<size_t>& channelSizeFieldOffsets,
            const std::vector<uint32_t>& newSizes,
            const size_t layerMaskSectionSizeOffset,
            const size_t layerMaskSectionNewSize
        );
    };
}

#endif //SPLITVCS_PSDCHANNELSIZEUPDATER_H
