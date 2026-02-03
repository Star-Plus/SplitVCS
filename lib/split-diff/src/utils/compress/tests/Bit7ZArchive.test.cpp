//
// Created by Ahmed Mustafa on 2/2/2026.
//

#include <gtest/gtest.h>

#include "utils/compress/Bit7Archive.h"

TEST(BitArchiveTest, TestCompressingDirectory)
{
    const std::string rootPath = "test_archive/";

    const Split::Bit7Archive archiver;

    archiver.SaveDirToArchive(rootPath + "test/", rootPath + "test.7z");

}

TEST(BitArchiveTest, TestExtractingDirectory)
{
    const std::string rootPath = "test_archive/";

    const Split::Bit7Archive archiver;

    archiver.ExtractArchive(rootPath + "test.7z", rootPath + "test/");
}