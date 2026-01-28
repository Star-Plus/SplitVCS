//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include "TinyAsset.h"

#include <filesystem>
#include <fstream>

#include "stores/Pack.h"
#include "utils/String/StringUtils.h"

namespace Split
{
    TinyAsset::TinyAsset(const str& rootPath) : rootPath(rootPath), tmpDecodePath(rootPath+"/.split/tmp/decode"), tmpCompressPath(rootPath+"/.split/tmp/compressed"), logger(true, "Tiny Asset")
    {
        if (tmpCompressPath.empty() | tmpDecodePath.empty())
        {
            throw std::invalid_argument("TinyAsset : invalid construction arguments");
        }

        std::filesystem::create_directories(tmpCompressPath);
        std::filesystem::create_directories(tmpDecodePath);

        load();
    }

    void TinyAsset::load()
    {
        const std::string tinyAssetFilePath = rootPath + "/.split/.tiny-map";
        std::fstream assetMapFile(tinyAssetFilePath, std::ios::in);
        if (!assetMapFile.is_open())
        {
            assetMapFile.open(tinyAssetFilePath, std::ios::out);
            assetMapFile.close();
        }

        std::string line;
        while (std::getline(assetMapFile, line))
        {
            const auto parts = StringUtils::split(line, ":");
            assetMap.insert({parts[0], parts[1]});
        }

        assetMapFile.close();
    }

    str TinyAsset::encodeAsset(
            const str& versionHash,
            const str& fileType,
            int quality
    ) const
    {
        if (const auto it = assetMap.find(versionHash); it != assetMap.end()) return it->second;

        Pack pack(this->rootPath);

        const auto packUnit = pack.getBasePackByHash(versionHash);

        const auto fileExtension = fileType.empty() ? "" : "." + fileType;
        logger.debug(fileExtension);

        const str decodedFilePath = tmpDecodePath + "/" + versionHash + fileExtension;
        const str compressedAssetPath =tmpCompressPath + "/" + versionHash;

        pack.decode(versionHash, decodedFilePath);

        auto resultPath = encoder.encode(decodedFilePath, compressedAssetPath, {quality, packUnit.encodeType});
        return resultPath;
    }
} // Split