//
// Created by Ahmed Mustafa on 1/22/2026.
//

#ifndef SPLITVCS_TINYASSET_H
#define SPLITVCS_TINYASSET_H
#include "core/Alias.h"
#include "split-live/src/LiveEncoder.h"
#include <map>

#include "main/Logger.h"

namespace Split
{
    class TinyAsset
    {
    public:
        explicit TinyAsset(const str&  rootPath);

        str encodeAsset(
            const str& versionHash,
            const str& fileType = "",
            int quality = 20
        ) const;

    private:
        str rootPath;
        LiveEncoder encoder;
        str tmpDecodePath, tmpCompressPath;
        std::map<std::string, std::string> assetMap;

        Logger logger;

        void load();
    };
} // Split

#endif //SPLITVCS_TINYASSET_H