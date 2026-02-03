//
// Created by Ahmed Mustafa on 1/31/2026.
//

#ifndef SPLITVCS_PSDLAYERSMETADATA_H
#define SPLITVCS_PSDLAYERSMETADATA_H

#include <string>
#include <set>

#include "utils/stream/OffsetBound.h"

namespace Split
{
    struct ChannelMetadata
    {
        OffsetBound offset;
    };

    inline bool operator < (const ChannelMetadata& lhs, const ChannelMetadata& rhs)
    {
        return lhs.offset < rhs.offset;
    }

    inline void operator << (std::ostream& os, const ChannelMetadata& channelMetadata)
    {
        os << channelMetadata.offset;
    }

    struct LayerMetadata
    {
        std::string name;
        std::string storePath;
        std::set<ChannelMetadata> channels;
    };

    inline bool operator < (const LayerMetadata& lhs, const LayerMetadata& rhs)
    {
        return lhs.channels.rbegin()->offset < rhs.channels.begin()->offset;
    }

    inline void operator << (std::ostream& os, const LayerMetadata& layerMetadata)
    {
        os << layerMetadata.name << "\n";
        os << layerMetadata.storePath << "\n";
        os << layerMetadata.channels.size() << "\n";
        for (auto channelMetadata : layerMetadata.channels)
        {
            os << channelMetadata;
            os << "\n";
        }
    }

    struct PsdLayersMetadata
    {
        std::set<LayerMetadata> layers;
    };

    inline void operator << (std::ostream& os, const PsdLayersMetadata& layerMetadata)
    {
        os << layerMetadata.layers.size() << "\n";
        for (const auto& value : layerMetadata.layers)
        {
            os << value;
        }
    }
}

#endif //SPLITVCS_PSDLAYERSMETADATA_H