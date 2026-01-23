//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include "TinyAsset.h"

#include <filesystem>

#include "stores/Pack.h"
#include <fstream>

namespace Split
{
    TinyAsset::TinyAsset(const str& rootPath) : rootPath(rootPath), tmpDecodePath(rootPath+"/.split/tmp/decode"), tmpCompressPath(rootPath+"/.split/tmp/compressed")
    {
        if (tmpCompressPath.empty() | tmpDecodePath.empty())
        {
            throw std::invalid_argument("TinyAsset : invalid construction arguments");
        }

        std::filesystem::create_directories(tmpCompressPath);
        std::filesystem::create_directories(tmpDecodePath);
    }

    str TinyAsset::encodeAsset(
            const str& versionHash,
            int quality
    ) const
    {
        Pack pack(this->rootPath);

        const auto packUnit = pack.getBasePackByHash(versionHash);

        const str decodedFilePath = tmpDecodePath + "/" + versionHash;
        const str compressedAssetPath =tmpCompressPath + "/" + versionHash;

        std::fstream outFile(decodedFilePath, std::ios::out | std::ios::binary);

        pack.decode(versionHash, outFile);
        outFile.close();

        auto resultPath = encoder.encode(decodedFilePath, compressedAssetPath, {quality, packUnit.encodeType});
        return resultPath;
    }
} // Split