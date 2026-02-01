//
// Created by Ahmed Mustafa on 1/31/2026.
//

#ifndef SPLITVCS_PSDLAYERMETADATAPARSER_H
#define SPLITVCS_PSDLAYERMETADATAPARSER_H
#include "PsdLayersMetadata.h"


namespace Split
{
    class PsdLayerMetadataParser
    {

    public:

        static OffsetBound parseOffsetBounds(std::istream& in)
        {
            size_t offset = 0, length = 0;

            std::string line;
            if (std::getline(in, line))
            {
                offset = std::stoi(line);
            }

            if (std::getline(in, line))
            {
                length = std::stoi(line);
            }

            const OffsetBound offsetBounds(offset, length);

            return offsetBounds;
        }

        PsdLayersMetadata parse(std::istream& in)
        {
            PsdLayersMetadata metadata;

            std::string line;

            int numOfLayers = 0;

            if (std::getline(in, line))
            {
                metadata.layerMaskSectionSizeOffset = std::stoi(line);
            }

            if (std::getline(in, line))
            {
                metadata.layerMaskSectionSize = std::stoi(line);
            }

            if (std::getline(in, line))
            {
                numOfLayers = std::stoi(line);
            }

            for (int i = 0; i < numOfLayers; i++)
            {
                LayerMetadata layerMetadata;
                std::string layerName;

                if (std::getline(in, layerName)) layerMetadata.name = layerName;


                int numOfChannels = 0;
                if (std::getline(in, line)) numOfChannels = std::stoi(line);

                for (int j = 0; j < numOfChannels; j++)
                {
                    ChannelMetadata channelMetadata;
                    channelMetadata.offset = parseOffsetBounds(in);

                    // Read compression type
                    if (std::getline(in, line))
                    {
                        channelMetadata.compressionType = static_cast<uint16_t>(std::stoi(line));
                    }

                    // Read channel size field offset
                    if (std::getline(in, line))
                    {
                        channelMetadata.channelSizeFieldOffset = std::stoi(line);
                    }

                    // Read whether this is an alpha channel
                    if (std::getline(in, line))
                    {
                        channelMetadata.isAlpha = std::stoi(line) != 0;
                    }

                    layerMetadata.channels.insert(channelMetadata);
                }

                metadata.layers.insert(layerMetadata);
            }

            return metadata;
        }

    };


}

#endif //SPLITVCS_PSDLAYERMETADATAPARSER_H