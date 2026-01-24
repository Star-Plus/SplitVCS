//
// Created by Ahmed Mustafa on 8/15/2025.
//

#include "Pack.h"
#include "atoms/Blob.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stack>
#include "ObjectStore.h"
#include "utils/Fs/DualStream.h"
#include "components/DeltaCompressor.h"
#include "utils/Fs/PackOptimizer.h"
#include "enums/AssetType.h"

namespace Split {

    bool isPackUnitByHash(const PackUnit &pack, const str &hash) {
        return pack.hash == hash;
    }

    bool isPackUnitByBaseHash(const PackUnit &pack, const str &baseHash) {
        return pack.baseHash == baseHash;
    }

    Pack::Pack(const str& rootPath) : rootPath(rootPath), path(rootPath + "/.split/refs/packs") {

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

    void Pack::decode(const str& hash, std::ostream& out) {

        auto it = std::ranges::find_if(packs.begin(), packs.end(), [&hash](const auto& p) {
            return isPackUnitByHash(*p, hash);
        });

        if (it == packs.end()) {
            throw std::runtime_error("Pack " + hash + " not found");
        }

        const ObjectStore blobsObjectStore(rootPath, "/blobs");
        const ObjectStore deltasObjectStore(rootPath, "/deltas");

        str baseHash;

        std::stack<std::unique_ptr<Blob>> deltaStack;
        std::stack<std::ifstream*> deltaFiles;

        auto tempRef = *it;
        auto tailRef = *it;

        while (tempRef->baseRef != nullptr)
        {
            tailRef = tempRef;

            const auto objectHash = tempRef->deltaHash;
            if (!deltasObjectStore.hasObject(objectHash)) {
                throw std::runtime_error("Object not found in deltas: " + objectHash);
            }

            auto deltaStream = new std::ifstream(deltasObjectStore.loadObject(objectHash));
            auto blob = std::make_unique<Blob>(*deltaStream, tempRef->encodeType);
            deltaStack.push(std::move(blob));
            deltaFiles.push(deltaStream);

            tempRef = tempRef->baseRef;
        }

        baseHash = tailRef->baseHash;

        DeltaCompressor compressor;

        if (!blobsObjectStore.hasObject(baseHash)) {
            throw std::runtime_error("Base object not found: " + hash);
        }

        auto baseStream = blobsObjectStore.loadObject(baseHash);
        Blob baseBlob(baseStream), outBlob(out);

        compressor.decode(baseBlob, deltaStack, outBlob);

        while (!deltaFiles.empty())
        {
            auto d = deltaFiles.top();
            d->close();
            deltaFiles.pop();
            delete d;
        }
    }

    str Pack::encodeBase(std::fstream& file, AssetType encodeType)
    {
        const ObjectStore blobObjectStore(rootPath, "/blobs");

        auto hash = blobObjectStore.storeFileObject(file);

        const auto pack = std::make_shared<PackUnit>(hash, hash, "", nullptr, encodeType);

        packs.push_back(pack);
        savePack(*pack);
        return hash;
    }

    str Pack::encodeDelta(std::istream& v2, const str& baseHash, const str& targetHash, AssetType encodeType) {

        const auto it = std::ranges::find_if(packs, [&baseHash](const auto& p)
        {
            return isPackUnitByHash(*p, baseHash);
        });

        if (it == packs.end())
        {
            throw std::runtime_error("Pack " + baseHash + " not found");
        }

        // Decode baseContent first

        DualStream v1;

        if (PackOptimizer::approximateDecodeSize(rootPath, *it) > DECODE_MAX_SIZE)
        {
            v1.emplaceFile(path + "/" + baseHash + ".tmp", std::ios::binary | std::ios::in | std::ios::out);
        } else
        {
            v1.emplaceMemory();
        }

        this->decode(baseHash, v1.out());

        // Start encoding

        const ObjectStore deltasObjectStore(rootPath, "/deltas");

        DeltaCompressor compressor;

        std::ostringstream oss;

        const Blob v1Blob(v1.in(), encodeType), v2Blob(v2, encodeType), oBlob(oss);

        compressor.encode(v1Blob, v2Blob, oBlob);
        if (oss.rdbuf()->str().empty()) {
            throw std::runtime_error("Failed to encode delta.");
        }

        // Save the delta to the deltas object store
        str deltaHash = deltasObjectStore.storeBytesObject(oss.rdbuf()->str());
        if (deltaHash.empty()) {
            throw std::runtime_error("Failed to store delta object.");
        }

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
        newPackUnit->hash = targetHash;
        newPackUnit->deltaHash = deltaHash;
        newPackUnit->encodeType = encodeType;

        packs.push_back(newPackUnit);

        savePack(*newPackUnit);
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

}
