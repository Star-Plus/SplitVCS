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
                numOfLayers = std::stoi(line);
            }

            for (int i = 0; i < numOfLayers; i++)
            {
                LayerMetadata layerMetadata;

                if (std::string layerName; std::getline(in, layerName)) layerMetadata.name = layerName;
                if (std::string storePath; std::getline(in, storePath)) layerMetadata.storePath = storePath;

                int numOfChannels = 0;
                if (std::getline(in, line)) numOfChannels = std::stoi(line);

                for (int j = 0; j < numOfChannels; j++)
                {
                    ChannelMetadata channelMetadata;
                    channelMetadata.offset = parseOffsetBounds(in);

                    layerMetadata.channels.insert(channelMetadata);
                }

                metadata.layers.insert(layerMetadata);
            }

            return metadata;
        }

    };


}

#endif //SPLITVCS_PSDLAYERMETADATAPARSER_H