//
// Created by Ahmed Mustafa on 1/29/2026.
//

#ifndef SPLITVCS_CHANNELUTILS_H
#define SPLITVCS_CHANNELUTILS_H
#include "PsdSdk/PsdChannel.h"
#include "PsdSdk/PsdInterleave.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdLayerCanvasCopy.h"


namespace Split::ChannelUtils
{
	static constexpr unsigned int CHANNEL_NOT_FOUND = UINT_MAX;

    inline unsigned int FindChannel(const psd::Layer* layer, const int16_t type)
    {
        for (unsigned int i = 0; i < layer->channelCount; i++)
        {
            if (layer->channels[i].type == type) return i;
        }

        return CHANNEL_NOT_FOUND;
    }

    template <typename T, typename DataHolder>
    void* ExpandChannelToCanvas(psd::Allocator* allocator, const DataHolder* layer, const void* data, unsigned int canvasWidth, unsigned int canvasHeight)
    {
        T* canvasData = static_cast<T*>(allocator->Allocate(sizeof(T)*canvasWidth*canvasHeight, 16u));
        memset(canvasData, 0u, sizeof(T)*canvasWidth*canvasHeight);

        psd::imageUtil::CopyLayerData(static_cast<const T*>(data), canvasData, layer->left, layer->top, layer->right, layer->bottom, canvasWidth, canvasHeight);

        return canvasData;
    }

    static void* ExpandChannelToCanvas(const psd::Document* document, psd::Allocator* allocator, const psd::Layer* layer, const psd::Channel* channel)
    {
        if (document->bitsPerChannel == 8)
            return ExpandChannelToCanvas<uint8_t>(allocator, layer, channel->data, document->width, document->height);
        if (document->bitsPerChannel == 16)
            return ExpandChannelToCanvas<uint16_t>(allocator, layer, channel->data, document->width, document->height);
        if (document->bitsPerChannel == 32)
            return ExpandChannelToCanvas<float32_t>(allocator, layer, channel->data, document->width, document->height);

        return nullptr;
    }


    template <typename T>
    T* CreateInterleavedImage(psd::Allocator* allocator, const void* srcR, const void* srcG, const void* srcB, unsigned int width, unsigned int height)
    {
        T* image = static_cast<T*>(allocator->Allocate(width*height * 4u*sizeof(T), 16u));

        const T* r = static_cast<const T*>(srcR);
        const T* g = static_cast<const T*>(srcG);
        const T* b = static_cast<const T*>(srcB);
        psd::imageUtil::InterleaveRGB(r, g, b, T(0), image, width, height);

        return image;
    }


    template <typename T>
    T* CreateInterleavedImage(psd::Allocator* allocator, const void* srcR, const void* srcG, const void* srcB, const void* srcA, unsigned int width, unsigned int height)
    {
        T* image = static_cast<T*>(allocator->Allocate(width*height * 4u*sizeof(T), 16u));

        const T* r = static_cast<const T*>(srcR);
        const T* g = static_cast<const T*>(srcG);
        const T* b = static_cast<const T*>(srcB);
        const T* a = static_cast<const T*>(srcA);
        psd::imageUtil::InterleaveRGBA(r, g, b, a, image, width, height);

        return image;
    }
}


#endif //SPLITVCS_CHANNELUTILS_H