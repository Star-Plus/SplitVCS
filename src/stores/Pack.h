//
// Created by Ahmed Mustafa on 8/15/2025.
//

#ifndef PACK_H
#define PACK_H

#include <vector>
#include <string>
#include <memory>

#include "atoms/Asset.h"
#include "core/Alias.h"
#include "enums/EncodeType.h"
#include "main/Logger.h"

namespace Split {

    struct PackUnit {
        str hash;
        str baseHash;
        str deltaHash;
        std::shared_ptr<PackUnit> baseRef = nullptr;
        EncodeType encodeType = EncodeType::BINARY;
    };

    class Pack {

    public:

        explicit Pack(const str& rootPath);
        void savePack(const PackUnit&) const;
        void decode(const str&, const std::string&);
        str encodeBase(std::fstream&, EncodeType=EncodeType::BINARY);
        str encodeDelta(const std::string& v2Path, const str& baseHash, const str& v2Hash, EncodeType encodeType=EncodeType::BINARY);

        PackUnit getPackUnitByHash(const str& hash) const;
        PackUnit getBasePackByHash(const str& hash) const;

    private:

        std::string rootPath;
        std::string path;
        std::vector<std::shared_ptr<PackUnit>> packs;

        Logger logger;

        size_t const DECODE_MAX_SIZE = 1024 * 1024 * 50;

        std::pair<Asset, std::vector<Asset>> fetchPacksToAsset(const std::string& hash) const;

    };

}



#endif //PACK_H
