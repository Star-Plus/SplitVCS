//
// Created by Ahmed Mustafa on 8/13/2025.
//

#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <map>
#include <cstdint>

namespace Split {

    struct IndexEntry {
        std::string filePath;
        std::string blobHash;
        std::string baseVersionHash;
        uint32_t mode; // File mode (permissions)
        uint64_t size;
        uint64_t mtime; // Last modified time
        bool isDeleted;
    };

    class Index {

        std::string path;
        std::string rootPath;
        std::map<std::string, IndexEntry> entries; // Maps file paths to their base version hashes

    public:

        explicit Index(const std::string& rootPath);

        void stageFile(const std::string& filepath);
        std::map<std::string, std::string> getStagedFiles() const;
        std::map<std::string, IndexEntry> getEntries() const { return entries; }
        void updateEntry(const std::string& filepath, const IndexEntry& entry);
        void removeEntry(const std::string& filepath);
        void save() const;
        void removeStagedFile(const std::string& filepath);

    };

}

#endif //INDEX_H
