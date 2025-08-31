//
// Created by Ahmed Mustafa on 8/15/2025.
//

#ifndef PACK_H
#define PACK_H

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

#include "core/Alias.h"

namespace Split {

    struct PackUnit {
        str hash;
        str baseHash;
        str deltaHash;
        std::shared_ptr<PackUnit> baseRef = nullptr;
    };

    class Pack {

        std::string rootPath;
        std::string path;
        std::vector<std::shared_ptr<PackUnit>> packs;

    public:

        Pack(const str& rootPath);
        void savePack(const str &hash) const;
        str getDecodedContent(const str&);
        str encodeDelta(const str& baseBytes, const str& targetBytes, const str& baseHash, const str& targetHash);

        PackUnit getPackUnitByHash(const str& hash) const;
        str getBaseVersionHash(const str& hash) const;

    };

}



#endif //PACK_H
