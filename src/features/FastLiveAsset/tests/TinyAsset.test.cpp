//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include <filesystem>
#include <gtest/gtest.h>
#include "features/FastLiveAsset/TinyAsset.h"
#include <fstream>
#include "components/Repository.h"

TEST(TinyAssetTest, TestCompressImage)
{
    const std::string rootPath = "image_dc_ta";
    try
    {
        std::fstream originalImage("image.png", std::ios::in | std::ios::binary);
        std::fstream editedImage("Brightness/edited.png", std::ios::in | std::ios::binary);

        Split::Repository repository(rootPath);
        repository.init();

        const std::string testImagePath = rootPath + "/test_image.png";

        std::fstream testImage(testImagePath, std::ios::out | std::ios::binary);
        testImage << originalImage.rdbuf();
        testImage.close();

        repository.add("test_image.png");

        // Remove testImage content and put editedImage
        std::fstream newImage(testImagePath, std::ios::out | std::ios::trunc | std::ios::binary);
        newImage << editedImage.rdbuf();

        newImage.close();

        repository.add("test_image.png");

        const auto hash = Split::Hashing::computeFileHash(testImagePath);

        Split::TinyAsset testAsset(rootPath);
        const auto compressedPath = testAsset.encodeAsset(hash);

        std::fstream compressedFile(compressedPath, std::ios::in | std::ios::binary);
        ASSERT_TRUE(compressedFile.good());

    } catch (...)
    {
        std::filesystem::remove_all(rootPath);
        ASSERT_TRUE(false);
    }
}

TEST(TinyAssetTest, TestAssetCache)
{
    Split::Logger logger(true, "Main test");

    const std::string rootPath = "image_dc_ta";
    std::filesystem::remove_all(rootPath);

    std::fstream originalImage("image.png", std::ios::in | std::ios::binary);
    std::fstream editedImage("Brightness/edited.png", std::ios::in | std::ios::binary);

    Split::Repository repository(rootPath);
    repository.init();

    const std::string testImagePath = rootPath + "/test_image.png";

    std::fstream testImage(testImagePath, std::ios::out | std::ios::binary);
    testImage << originalImage.rdbuf();
    testImage.close();

    repository.add("test_image.png");

    // Remove testImage content and put editedImage
    std::fstream newImage(testImagePath, std::ios::out | std::ios::trunc | std::ios::binary);
    newImage << editedImage.rdbuf();

    newImage.close();

    repository.add("test_image.png");

    const auto hash = Split::Hashing::computeFileHash(testImagePath);

    Split::TinyAsset testAsset(rootPath);
    const auto compressedPath = testAsset.encodeAsset(hash);
    logger.debug("Compressed 1");

    std::fstream compressedFile(compressedPath, std::ios::in | std::ios::binary);
    ASSERT_TRUE(compressedFile.good());

    const auto compressedImagePathAgain = testAsset.encodeAsset(hash);
    logger.debug("Compressed 2");
}