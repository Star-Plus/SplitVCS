//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include "PsdEncoder.h"

#include <filesystem>
#include <fstream>
#include <map>
#include <set>

#include "features/dissolve/AssetDissolver.h"
#include "PsdSdk/PsdDocument.h"
#include "PsdSdk/PsdLayerMaskSection.h"
#include "PsdSdk/PsdMallocAllocator.h"
#include "PsdSdk/PsdNativeFile.h"
#include "PsdSdk/PsdParseDocument.h"
#include "PsdSdk/PsdParseLayerMaskSection.h"
#include "PsdSdk/PsdLayer.h"
#include "PsdSdk/PsdExport.h"
#include "utils/psd/ChannelUtils.h"
#include "utils/stream/OffsetBound.h"


namespace Split
{
    std::string PsdEncoder::encode(const std::string& base, const std::string& out)
    {
        psd::MallocAllocator allocator;
        psd::NativeFile file(&allocator);
        std::filesystem::path filepath(base);

        if (!file.OpenRead(filepath.wstring().c_str()))
            throw std::runtime_error("File not found");

        psd::Document* document = psd::CreateDocument(&file, &allocator);
        psd::LayerMaskSection* layerMask = psd::ParseLayerMaskSection(document, &file, &allocator);

        std::map<const std::string, std::vector<OffsetBound>> excludeSet;
        std::set<OffsetBound> fileExcludeSet;

        for (unsigned int i = 0; i < layerMask->layerCount; i++)
        {
            psd::Layer* layer = &layerMask->layers[i];

            auto exportedLayers = psdAdapter.pixelsToMat(document, file, allocator, layer);
            const std::string savePrefix = out + "-" + layer->name.c_str();
            cv::imwrite(savePrefix + ".webp", exportedLayers.rgba, {cv::IMWRITE_WEBP_QUALITY, 101});

            std::vector<OffsetBound> excludes;

            for (unsigned int j = 0; j < 4; j++)
            {
                const auto channel = layer->channels[j];
                const OffsetBound exclude(channel.fileOffset, channel.size);
                excludes.push_back(exclude);
                fileExcludeSet.insert(exclude);
            }

            excludeSet.insert({layer->name.c_str(), excludes});
        }

        AssetDissolver dissolver;
        dissolver.dissolve(base, out + "-psd", fileExcludeSet);

        // Cleanup
        psd::DestroyLayerMaskSection(layerMask, &allocator);
        psd::DestroyDocument(document, &allocator);
        file.Close();

        std::ofstream metadataFile(out,std::ios::binary);

        for (auto entry : excludeSet)
        {
            metadataFile << entry.first << "\n";
            for (auto exclude : entry.second)
            {
                metadataFile << exclude.offset;
                metadataFile << "\n";
            }
        }

        metadataFile.close();

        return out;
    }

    std::string PsdEncoder::encode(const std::string& base, std::stack<std::string>& deltas, const std::string& v2, std::string& out)
    {
        return "";
    }

    void PsdEncoder::encode(const std::istream& v1, const std::istream& v2, std::ostream& out)
    {
    }
} // Split