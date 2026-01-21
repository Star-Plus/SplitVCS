//
// Created by Ahmed Mustafa on 8/15/2025.
//

#include "Pack.h"
#include <filesystem>
#include <fstream>
#include "ObjectStore.h"
#include "components/DeltaCompressor.h"

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

                if (std::getline(file, deltaHash)) {
                    if (deltaHash.empty()) {
                        throw std::runtime_error("Base hash is empty in pack file: " + entry.path().string());
                    }
                } else {
                    throw std::runtime_error("Failed to read base hash from pack file: " + entry.path().string());
                }

                file.close();

                auto packUnit = std::make_shared<PackUnit>();
                packUnit->hash = hash;
                packUnit->baseHash = baseHash;
                packUnit->deltaHash = deltaHash;
                packUnit->baseRef = nullptr;
                packs.push_back(packUnit);
            }
        }

        for (auto &pack : packs) {
            if (pack->baseHash.empty()) {
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

    void Pack::savePack(const str &hash) const {
        const auto it = std::ranges::find_if(packs, [&hash](const auto& p) {
            return isPackUnitByHash(*p, hash);
        });

        if (it == packs.end()) {
            return;
        }

        const str filePath = rootPath + "/.split/refs/packs/" + hash + ".pack";
        std::ofstream file(filePath);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open pack file for writing: " + filePath);
        }

        const str &baseHashStr = (*it)->baseHash;
        file << baseHashStr << "\n";
        const str &deltaHash = (*it)->deltaHash;
        file << deltaHash << "\n";

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

        std::stack<std::unique_ptr<Blob>> deltaStack = {};

        auto tempRef = *it;
        auto tailRef = *it;

        while (tempRef->baseRef != nullptr) {
            tailRef = tempRef;

            const auto objectHash = tempRef->deltaHash;
            if (!deltasObjectStore.hasObject(objectHash)) {
                throw std::runtime_error("Object not found in deltas: " + objectHash);
            }

            std::ifstream deltaStream = deltasObjectStore.loadObject(objectHash);
            deltaStack.push(std::make_unique<Blob>(Blob(&deltaStream)));

            tempRef = tempRef->baseRef;
        }

        baseHash = tailRef->baseHash;

        DeltaCompressor compressor;

        if (!blobsObjectStore.hasObject(baseHash)) {
            throw std::runtime_error("Base object not found: " + hash);
        }

        auto baseStream = blobsObjectStore.loadObject(baseHash);
        Blob baseBlob(&baseStream), outBlob(&out);

        compressor.decode(baseBlob, deltaStack, outBlob);
    }

    str Pack::encodeDelta(std::istream& v1, std::istream& v2, const str& baseHash, const str& targetHash) {
        const ObjectStore deltasObjectStore(rootPath, "/deltas");

        DeltaCompressor compressor;

        std::ostringstream oss;

        const Blob v1Blob(&v1), v2Blob(&v2), oBlob(&oss);

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

        packs.push_back(newPackUnit);

        savePack(targetHash);
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

    str Pack::getBaseVersionHash(const str &hash) const {
        const auto it = std::ranges::find_if(packs, [&hash](const auto& p) {
            return isPackUnitByHash(*p, hash);
        });

        if (it == packs.end()) {
            return hash;
        }

        auto tempRef = (*it)->baseRef;
        auto trailRef = *it;

        while (tempRef != nullptr) {
            trailRef = tempRef;
            tempRef = tempRef->baseRef;
        }

        return trailRef->baseHash;
    }

}
