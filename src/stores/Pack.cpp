//
// Created by Ahmed Mustafa on 8/15/2025.
//

#include "Pack.h"
#include "atoms/Blob.h"
#include <filesystem>
#include <fstream>
#include "ObjectStore.h"
#include "enums/EncodeType.h"
#include "utils/Hashing.h"
#include "utils/String/StringUtils.h"

namespace Split {

    bool isPackUnitByHash(const PackUnit &pack, const str &hash) {
        return pack.hash == hash;
    }

    bool isPackUnitByBaseHash(const PackUnit &pack, const str &baseHash) {
        return pack.baseHash == baseHash;
    }

    Pack::Pack(const str& rootPath) : rootPath(rootPath), path(rootPath + "/.split/refs/packs"), logger(true, "Pack") {

        // Ensure the directory exists
        std::filesystem::create_directories(path);

        // Load existing packs from the directory
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                str hash = entry.path().stem().string();

                str baseHash;
                str deltaHash;
                str encodeType;

                std::ifstream file(entry.path());

                if (!file.is_open()) {
                    throw std::runtime_error("Failed to open pack file: " + entry.path().string());
                }

                if (std::getline(file, baseHash)) {
                    if (baseHash.empty()) {
                        throw std::runtime_error("Base hash is empty in pack file: " + entry.path().string());
                    }
                } else {
                    throw std::runtime_error("Failed to read base hash from pack file: " + entry.path().string());
                }

                if (!std::getline(file, deltaHash)) {
                    throw std::runtime_error("Failed to read base hash from pack file: " + entry.path().string());
                }

                if (!std::getline(file, encodeType))
                {
                    encodeType = "binary";
                }

                file.close();

                auto packUnit = std::make_shared<PackUnit>();
                packUnit->hash = hash;
                packUnit->baseHash = baseHash;
                packUnit->deltaHash = deltaHash;
                packUnit->baseRef = nullptr;
                packUnit->encodeType = Assets::typeFromString(encodeType);
                packs.push_back(packUnit);
            }
        }

        for (auto &pack : packs) {
            if (pack->baseHash.empty() || pack->deltaHash.empty()) {
                continue;
            }

            auto baseIt = std::ranges::find_if(packs, [&pack](const auto& p) {
                return isPackUnitByHash(*p, pack->baseHash);
            });

            if (baseIt != packs.end()) {
                pack->baseRef = *baseIt;
            }
        }
    }

    void Pack::savePack(const PackUnit& pack) const {

        const str filePath = rootPath + "/.split/refs/packs/" + pack.hash + ".pack";
        std::ofstream file(filePath);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open pack file for writing: " + filePath);
        }

        file << pack.baseHash << "\n";
        file << pack.deltaHash << "\n";
        file << Assets::typeToString(pack.encodeType);

        file.close();
    }

    void Pack::decode(const str& hash, const std::string& out) const
    {
        const auto packLine = this->fetchPacksToAsset(hash);
        Asset outAsset(out);
        compressor.decode(packLine.first, packLine.second, outAsset);
    }

    str Pack::encodeBase(const std::string& file, EncodeType encodeType)
    {
        const ObjectStore blobObjectStore(rootPath, "/blobs");

        const std::string fileFullPath = rootPath + "/" + file;

        auto hash = Hashing::computeFileHash(fileFullPath);

        compressor.encode(Asset{fileFullPath}, Asset{blobObjectStore.getPath() + "/" + hash});

        const auto pack = std::make_shared<PackUnit>(hash, hash, "", nullptr, encodeType);

        packs.push_back(pack);
        savePack(*pack);
        return hash;
    }

    str Pack::encodeDelta(const std::string& v2Path, const str& baseHash, const str& v2Hash, EncodeType encodeType) {
        const ObjectStore deltasObjectStore(rootPath, "/deltas");

        const auto packLine = this->fetchPacksToAsset(baseHash);

        const auto deltaPath = deltasObjectStore.getPath() + "/" + v2Hash;
        Asset outAsset(deltaPath);
        const auto producedPath = compressor.encode(packLine.first, packLine.second, Asset(v2Path , encodeType), outAsset);

        auto producedPathParts = StringUtils::split(producedPath, "/\\");
        const std::string deltaHash = producedPathParts.back();

        const auto baseIt = std::ranges::find_if(packs, [&baseHash](const auto& p) {
            return isPackUnitByHash(*p, baseHash);
        });

        const auto newPackUnit = std::make_shared<PackUnit>();

        if (baseIt != packs.end()) {
            newPackUnit->baseRef = *baseIt;
        } else {
            newPackUnit->baseRef = nullptr;
        }

        newPackUnit->baseHash = baseHash;
        newPackUnit->hash = v2Hash;
        newPackUnit->deltaHash = deltaHash;
        newPackUnit->encodeType = encodeType;

        packs.push_back(newPackUnit);

        savePack(*newPackUnit);

        logger.debug("End pack encoding delta");

        return deltaHash;
    }

    PackUnit Pack::getPackUnitByHash(const str& hash) const {
        const auto it = std::ranges::find_if(packs, [&hash](const auto& p) {
            return isPackUnitByHash(*p, hash);
        });

        if (it == packs.end()) {
            return PackUnit{hash, "", "", nullptr};
        }

        return **it;
    }

    PackUnit Pack::getBasePackByHash(const str &hash) const {
        const auto it = std::ranges::find_if(packs, [&hash](const auto& p) {
            return isPackUnitByHash(*p, hash);
        });

        if (it == packs.end()) {
            throw std::runtime_error("Pack " + hash + " not found");
        }

        auto tempRef = (*it)->baseRef;
        auto trailRef = *it;

        while (tempRef != nullptr) {
            trailRef = tempRef;
            tempRef = tempRef->baseRef;
        }

        return *trailRef;
    }

    std::pair<Asset, std::vector<Asset>> Pack::fetchPacksToAsset(const std::string& hash) const
    {
        auto it = std::ranges::find_if(packs.begin(), packs.end(), [&hash](const auto& p) {
            return isPackUnitByHash(*p, hash);
        });

        if (it == packs.end()) {
            throw std::runtime_error("Pack " + hash + " not found");
        }

        ObjectStore deltasObjectStore(rootPath, "/deltas"), blobsObjectStore(rootPath, "/blobs");

        str baseHash;

        std::vector<Asset> deltaStack;

        auto tempRef = *it;
        auto tailRef = *it;

        while (tempRef->baseRef != nullptr)
        {
            tailRef = tempRef;

            const auto objectHash = tempRef->deltaHash;

            auto asset = Asset(rootPath + "/.split/objects/deltas/" + objectHash, tempRef->encodeType);
            deltaStack.push_back(asset);

            tempRef = tempRef->baseRef;
        }

        baseHash = tailRef->baseHash;

        Asset baseAsset(rootPath+"/.split/objects/blobs/" + baseHash, tailRef->encodeType);

        return {baseAsset, deltaStack};
    }
}
