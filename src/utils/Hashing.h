//
// Created by Ahmed Mustafa on 8/13/2025.
//

#ifndef HASHING_H
#define HASHING_H

#include <string>
#include <fstream>

#include "SHA1.h"

namespace Split {

    class Hashing {
    public:

        static std::string computeFileHash(const std::string& filePath) {
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file for hashing: " + filePath);
            }

            SHA1 sha1;
            char buffer[8192];
            while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
                sha1.update(reinterpret_cast<uint8_t*>(buffer), file.gcount());
            }

            file.close();

            return sha1.final();
        }

        static std::string computeFileHash(std::fstream& file)
        {
            SHA1 sha1;
            char buffer[8192];
            while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
                sha1.update(reinterpret_cast<uint8_t*>(buffer), file.gcount());
            }

            // Return file to begin
            file.clear();
            file.seekg(0, std::ios::beg);

            return sha1.final();
        }

        static std::string computeHash(const std::string& bytes) {
            SHA1 sha1;
            sha1.update(reinterpret_cast<const uint8_t*>(bytes.data()), bytes.size());
            return sha1.final();
        }

    };

}

#endif //HASHING_H
