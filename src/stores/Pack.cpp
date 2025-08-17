//
// Created by Ahmed Mustafa on 8/15/2025.
//

#include "Pack.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>
#include <utils/DeltaCompressor.h>
#include "ObjectStore.h"

namespace Split {

    bool isPackUnitByHash(const PackUnit &pack, const str &hash) {
        return pack.hash == hash;
    }

    bool isPackUnitByBaseHash(const PackUnit &pack, const str &baseHash) {
        return pack.baseHash == baseHash;
    }

    Pack::Pack(const str& rootPath) : path(rootPath + "/.split/refs/packs"), rootPath(rootPath) {

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

            auto baseIt = std::ranges::find_if(packs, [&pack](const auto p) {
                return isPackUnitByHash(*p, pack->baseHash);
            });

            if (baseIt != packs.end()) {
                pack->baseRef = *baseIt;
            } else {
                throw std::runtime_error("Base hash not found for pack: " + pack->hash);
            }
        }
    }

    void Pack::savePack(const str &hash) const {
        const auto it = std::ranges::find_if(packs, [&hash](const auto p) {
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

    str Pack::getDecodedContent(const str& hash) {

        auto it = std::ranges::find_if(packs.begin(), packs.end(), [&hash](const auto p) {
            return isPackUnitByHash(*p, hash);
        });

        if (it == packs.end()) {
            return "\n";
        }

        str baseHash;

        std::stack<str> deltaStack = (*it)->deltaHash.empty() ? std::stack<str>() : std::stack<str>({(*it)->deltaHash});

        auto tempRef = (*it)->baseRef;
        auto tailRef = (*it)->baseRef;

        while (tempRef != nullptr) {
            tailRef = tempRef;
            deltaStack.push(tempRef->deltaHash);
            tempRef = tempRef->baseRef;
        }

        baseHash = tailRef->baseHash;


        const ObjectStore blobsObjectStore(rootPath, "/blobs");
        const ObjectStore deltasObjectStore(rootPath, "/deltas");

        DeltaCompressor compressor;

        if (!blobsObjectStore.hasObject(baseHash)) {
            throw std::runtime_error("Base object not found: " + hash);
        }

        std::ostringstream oss;
        oss << blobsObjectStore.loadObject(baseHash).rdbuf();
        str content = oss.str();

        while (!deltaStack.empty()) {
            str objectHash = deltaStack.top();
            deltaStack.pop();

            if (!deltasObjectStore.hasObject(objectHash)) {
                throw std::runtime_error("Object not found in deltas: " + objectHash);
            }

            std::ostringstream deltaStream;
            deltaStream << deltasObjectStore.loadObject(objectHash).rdbuf();
            str deltaContent = deltaStream.str();

            try {
                content = compressor.decode(content, deltaContent);
            } catch (const std::runtime_error &e) {
                throw std::runtime_error("Failed to decode delta for object " + objectHash + ": " + e.what());
            }
        }

        return content;
    }

    str Pack::encodeDelta(const str& baseBytes, const str& targetBytes, const str& baseHash, const str& targetHash) {
        DeltaCompressor compressor;
        const auto delta = compressor.encode(baseBytes, targetBytes);
        if (delta.empty()) {
            throw std::runtime_error("Failed to encode delta.");
        }

        // Save the delta to the deltas object store
        const ObjectStore deltasObjectStore(rootPath, "/deltas");
        str deltaHash = deltasObjectStore.storeBytesObject(delta);
        if (deltaHash.empty()) {
            throw std::runtime_error("Failed to store delta object.");
        }

        const auto baseIt = std::ranges::find_if(packs, [&baseHash](const auto p) {
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

}
