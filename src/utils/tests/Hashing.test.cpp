//
// Created by Ahmed Mustafa on 2/4/2026.
//

#include <gtest/gtest.h>
#include <fstream>

#include "utils/Hashing.h"

TEST(HashingTest, HashPsdFile)
{
    std::fstream out("test_hashing/file", std::ios::out | std::ios::binary);
    out << Split::Hashing::computeFileHash("test_hashing/Sample.psd");
    out.close();
}