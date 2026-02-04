//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "components/Repository.h"
#include "main/Logger.h"

TEST(HandlingImage, IncreaseBrightness)
{

    Split::Logger logger(true, "Main Test");

    const std::string rootPath = "image_dc_1";

    logger.setCheckPoint();
    std::filesystem::remove_all(rootPath);
    logger.debug("Clear the working environment dir");

    std::fstream originalImage("image.png", std::ios::in | std::ios::binary);
    std::fstream brightEditedImage("Brightness/edited.png", std::ios::in | std::ios::binary);
    std::fstream tintEditedImage("Tint/edited.png", std::ios::in | std::ios::binary);

    Split::Repository repository(rootPath);
    repository.init();

    const std::string testImagePath = rootPath + "/test_image.png";

    std::fstream testImage(testImagePath, std::ios::out | std::ios::binary);
    testImage << originalImage.rdbuf();
    testImage.close();

    repository.add("test_image.png");
    logger.debug("Added first version");

    repository.commit("First version", "Ahmed");

    // Remove testImage content and put editedImage
    std::fstream edited1(testImagePath, std::ios::out | std::ios::trunc | std::ios::binary);
    edited1 << brightEditedImage.rdbuf();
    brightEditedImage.close();
    edited1.close();
    logger.debug("Rewrite the image with edit 1");

    repository.add("test_image.png");
    logger.debug("Added second version");

    repository.commit("2nd version", "Ahmed");

    std::fstream edited2(testImagePath, std::ios::out | std::ios::trunc | std::ios::binary);
    edited2 << tintEditedImage.rdbuf();
    tintEditedImage.close();
    edited2.close();
    logger.debug("Rewrite the image with edit 2");

    repository.add("test_image.png");
    logger.debug("Added third version");

    repository.commit("3rd version", "Ahmed");
}