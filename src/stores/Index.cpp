
#include "Index.h"

#include <utility>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "ObjectStore.h"

#include "Pack.h"
#include "../utils/Hashing.h"

namespace Split {

    Index::Index(const std::string& rootPath) :
        path(rootPath + "/.split/index"),
        rootPath(rootPath)
    {
        std::ifstream fs;
        fs.open(this->path, std::ios::in | std::ios::binary);

        if (!fs.is_open()) {
            fs.open(this->path, std::ios::out | std::ios::binary);
            fs.close();
        } else {
            IndexEntry entry = {};

            while (fs.peek() != EOF) {
                uint32_t pathLen;
                fs.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
                entry.filePath.resize(pathLen);
                fs.read(entry.filePath.data(), pathLen);

                uint32_t hashLen;
                fs.read(reinterpret_cast<char*>(&hashLen), sizeof(hashLen));
                entry.blobHash.resize(hashLen);
                fs.read(entry.blobHash.data(), hashLen);

                fs.read(reinterpret_cast<char*>(&hashLen), sizeof(hashLen));
                entry.baseVersionHash.resize(hashLen);
                fs.read(entry.baseVersionHash.data(), hashLen);

                fs.read(reinterpret_cast<char*>(&entry.mode), sizeof(entry.mode));
                fs.read(reinterpret_cast<char*>(&entry.size), sizeof(entry.size));
                fs.read(reinterpret_cast<char*>(&entry.mtime), sizeof(entry.mtime));
                fs.read(reinterpret_cast<char*>(&entry.isDeleted), sizeof(entry.isDeleted));

                entries[entry.filePath] = entry;
            }
            fs.close();
        }
    }

    void Index::stageFile(const std::string &filepath) {

        IndexEntry entry = {
            filepath, "", "", 0, 0, 0, false
        };

        ObjectStore objectStore(rootPath, "/blobs");

        const auto originalFilePath = rootPath + "/" + filepath;

        const std::string blobHash = Hashing::computeFileHash(filepath);

        std::fstream originalFs(originalFilePath, std::ios::in | std::ios::binary);
        if (!originalFs.is_open()) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        if (entries.find(filepath) != entries.end()) {
            if (entries[filepath].blobHash == blobHash) {
                return;
            }

            entry.baseVersionHash = entries[filepath].baseVersionHash;

            Pack pack(rootPath);
            std::stringstream decodedStream;
            pack.decode(entries[filepath].blobHash, decodedStream);

            pack.encodeDelta(decodedStream, originalFs, entries[filepath].blobHash, blobHash);
        }
        else {
            entry.baseVersionHash = blobHash;
        }

        originalFs.seekg(0, std::ios::end);
        entry.size = originalFs.tellg();
        originalFs.seekg(0, std::ios::beg);
        entry.mtime = std::filesystem::last_write_time(originalFilePath).time_since_epoch().count();
        originalFs.close();

        entry.blobHash = blobHash;

        entries[filepath] = entry;
        // Delete the created blob
        if (blobHash != entry.baseVersionHash) {
            if (objectStore.hasObject(blobHash)) {
                objectStore.deleteObject(blobHash);
            }
        }

        // Write the updated index to disk
        save();
    }

    std::map<std::string, std::string> Index::getStagedFiles() const {
        std::map<std::string, std::string> stagedFiles;
        for (const auto &pair : entries) {
            if (!pair.second.isDeleted) {
                stagedFiles[pair.first] = pair.second.blobHash;
            }
        }
        return stagedFiles;
    }

    void Index::updateEntry(const std::string &filepath, const IndexEntry &entry) {
        auto it = entries.find(filepath);
        if (it != entries.end()) {
            it->second = entry;
        } else {
            entries[filepath] = entry;
        }
    }

    void Index::removeEntry(const std::string &filepath) {
        auto it = entries.find(filepath);
        if (it != entries.end()) {
            entries.erase(it);
        }
    }

    void Index::removeStagedFile(const std::string &filepath) {
        auto it = entries.find(filepath);
        if (it != entries.end()) {
            it->second.isDeleted = true;
        }

        // Write the updated index to disk
        save();
    }

    void Index::save() const {
        std::ofstream indexFs(this->path);

        for (const auto &pair : entries) {
            const IndexEntry &e = pair.second;

            // Serialize filePath
            uint32_t pathLen = e.filePath.size();
            indexFs.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
            indexFs.write(e.filePath.data(), pathLen);

            // Serialize blobHash
            uint32_t hashLen = e.blobHash.size();
            indexFs.write(reinterpret_cast<const char*>(&hashLen), sizeof(hashLen));
            indexFs.write(e.blobHash.data(), hashLen);

            // Serialize baseVersionHash
            uint32_t baseHashLen = e.baseVersionHash.size();
            indexFs.write(reinterpret_cast<const char*>(&baseHashLen), sizeof(baseHashLen));
            indexFs.write(e.baseVersionHash.data(), baseHashLen);

            // Serialize other fields
            indexFs.write(reinterpret_cast<const char*>(&e.mode), sizeof(e.mode));
            indexFs.write(reinterpret_cast<const char*>(&e.size), sizeof(e.size));
            indexFs.write(reinterpret_cast<const char*>(&e.mtime), sizeof(e.mtime));
            indexFs.write(reinterpret_cast<const char*>(&e.isDeleted), sizeof(e.isDeleted));
        }

        indexFs.close();
    }


}
