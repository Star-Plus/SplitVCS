//
// Created by Ahmed Mustafa on 2/3/2026.
//

#include "PsdUltraviolet.h"

#include <filesystem>
#include <fstream>

#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdImageResourcesSection.h"
#include "PsdSdk/PsdMallocAllocator.h"
#include "PsdSdk/PsdNativeFile.h"
#include "PsdSdk/PsdParseDocument.h"
#include "PsdSdk/PsdParseImageResourcesSection.h"
#include "PsdSdk/PsdThumbnail.h"

namespace Split
{
    std::string PsdUltraviolet::encode(const std::string& src, const std::string& save, EncodeOptions options)
    {
        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);

        std::filesystem::path srcPath(src);

        if (!file.OpenRead(srcPath.wstring().c_str()))
            throw std::runtime_error("File not found");

        const psd::Document* document = psd::CreateDocument(&file, &allocator);
        if (!document)
            throw std::runtime_error("Failed to create document");

        const psd::ImageResourcesSection* imageResources = psd::ParseImageResourcesSection(document, &file, &allocator);
        if (!imageResources)
            throw std::runtime_error("Failed to parse image resources");

        psd::Thumbnail* thumbnail = imageResources->thumbnail;
        if (!thumbnail)
            throw std::runtime_error("Failed to create thumbnail");

        std::ofstream saveJPEGStream(save + ".jpg", std::ios::binary | std::ios::trunc);

        saveJPEGStream.write(reinterpret_cast<char*>(thumbnail->binaryJpeg), thumbnail->binaryJpegSize);
        saveJPEGStream.close();

        return save + ".jpg";
    }
} // Split