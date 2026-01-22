//
// Created by Ahmed Mustafa on 8/15/2025.
//

#ifndef PACK_H
#define PACK_H

#include <vector>
#include <string>
#include <memory>
#include "core/Alias.h"
#include "enums/AssetType.h"

namespace Split {

    struct PackUnit {
        str hash;
        str baseHash;
        str deltaHash;
        std::shared_ptr<PackUnit> baseRef = nullptr;
        AssetType encodeType = AssetType::BINARY;
    };

    class Pack {

    public:

        explicit Pack(const str& rootPath);
        void savePack(const PackUnit&) const;
        void decode(const str&, std::ostream&);
        str encodeBase(std::fstream&, AssetType=AssetType::BINARY);
        str encodeDelta(std::istream& v2, const str& baseHash, const str& targetHash, AssetType encodeType=AssetType::BINARY);

        PackUnit getPackUnitByHash(const str& hash) const;
        str getBaseVersionHash(const str& hash) const;

    private:

        std::string rootPath;
        std::string path;
        std::vector<std::shared_ptr<PackUnit>> packs;

        size_t const DECODE_MAX_SIZE = 1024 * 1024 * 50;

    };

}



#endif //PACK_H
