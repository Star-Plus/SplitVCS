//
// Created by Ahmed Mustafa on 2/2/2026.
//

#include "RandomHash.h"

#include <random>

namespace Split
{
    std::string RandomHash::generateHash(size_t length)
    {
        const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789";

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<> dis(0.0, chars.length() - 1);

        std::string randomHash;
        randomHash.reserve(length);

        for (size_t i = 0; i < length; i++)
        {
            randomHash += chars[dis(gen)];
        }

        return randomHash;
    }
} // Split