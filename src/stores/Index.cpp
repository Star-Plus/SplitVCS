
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
        std::fstream fs;
        fs.open(this->path, std::ios::in | std::ios::binary);

        if (!fs.is_open()) {
            fs.open(this->path, std::ios::out | std::ios::binary);
            fs.close();
        } else {
            IndexEntry entry;
            while (fs.read(reinterpret_cast<char*>(&entry.filePath), sizeof(entry.filePath))) {
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

        const std::string blobHash = objectStore.storeFileObject(filepath);

        std::fstream originalFs(filepath, std::ios::in | std::ios::binary);
        if (!originalFs.is_open()) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        if (entries.find(filepath) != entries.end()) {
            if (entries[filepath].blobHash == blobHash) {
                return;
            }
            entry.baseVersionHash = entries[filepath].baseVersionHash;

            Pack pack(rootPath);
            auto decodedContent = pack.getDecodedContent(entries[filepath].blobHash);

            if (decodedContent.empty()) {
                throw std::runtime_error("Failed to decode content for " + filepath);
            }

            std::ostringstream fileStream;
            fileStream << originalFs.rdbuf();
            std::string targetContent = fileStream.str();

            if (decodedContent == "\n") {
                auto baseBlobStream = objectStore.loadObject(entries[filepath].baseVersionHash);
                std::ostringstream baseBlobStringStream;
                baseBlobStringStream << baseBlobStream.rdbuf();
                decodedContent = baseBlobStringStream.str();
            }

            pack.encodeDelta(decodedContent, targetContent, entries[filepath].blobHash, blobHash);
        }
        else {
            entry.baseVersionHash = blobHash;
        }

        originalFs.seekg(0, std::ios::end);
        entry.size = originalFs.tellg();
        originalFs.seekg(0, std::ios::beg);
        entry.mtime = std::filesystem::last_write_time(filepath).time_since_epoch().count();
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

    std::map<std::string, std::string> Index::getStagedFiles() const {
        std::map<std::string, std::string> stagedFiles;
        for (const auto &pair : entries) {
            if (!pair.second.isDeleted) {
                stagedFiles[pair.first] = pair.second.blobHash;
            }
        }
        return stagedFiles;
    }

    void Index::removeStagedFile(const std::string &filepath) {
        auto it = entries.find(filepath);
        if (it != entries.end()) {
            it->second.isDeleted = true;
        }

        // Write the updated index to disk
        std::fstream fs(this->path, std::ios::out | std::ios::binary);

        for (const auto &pair : entries) {
            const IndexEntry &e = pair.second;

            // Serialize filePath
            uint32_t pathLen = e.filePath.size();
            fs.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
            fs.write(e.filePath.data(), pathLen);

            // Serialize blobHash
            uint32_t hashLen = e.blobHash.size();
            fs.write(reinterpret_cast<const char*>(&hashLen), sizeof(hashLen));
            fs.write(e.blobHash.data(), hashLen);

            // Serialize other fields
            fs.write(reinterpret_cast<const char*>(&e.mode), sizeof(e.mode));
            fs.write(reinterpret_cast<const char*>(&e.size), sizeof(e.size));
            fs.write(reinterpret_cast<const char*>(&e.mtime), sizeof(e.mtime));
            fs.write(reinterpret_cast<const char*>(&e.isDeleted), sizeof(e.isDeleted));
        }

        fs.close();
    }


}
