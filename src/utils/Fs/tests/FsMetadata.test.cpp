//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include <filesystem>
#include <gtest/gtest.h>
#include "utils/Fs/FsMetadata.h"


TEST(FsMetadataTest, SimpleFileCase)
{
    std::fstream file("test_file.txt", std::ios::out);
    std::string content = "Testing content to add some size to the file. Hello I'm Ahmed";
    file << content;
    file.close();

    auto size = Split::FsMetadata::calculateFileSize("test_file.txt");

    ASSERT_EQ(size, content.size());

    std::filesystem::remove_all("test_file.txt");
}