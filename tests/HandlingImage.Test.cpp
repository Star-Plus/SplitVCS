//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include <gtest/gtest.h>
#include <fstream>

#include "components/Repository.h"

TEST(HandlingImage, IncreaseBrightness)
{
    std::fstream originalImage("image.png", std::ios::in | std::ios::binary);
    std::fstream editedImage("Brightness/edited.png", std::ios::in | std::ios::binary);

    Split::Repository repository("image_dc_1");
    repository.init();

    const std::string testImagePath = "image_dc_1/test_image.png";

    std::fstream testImage(testImagePath, std::ios::out | std::ios::binary);
    testImage << originalImage.rdbuf();
    testImage.close();

    repository.add("test_image.png");

    // Remove testImage content and put editedImage
    std::fstream newImage(testImagePath, std::ios::out | std::ios::trunc | std::ios::binary);
    newImage << editedImage.rdbuf();

    newImage.close();

    repository.add("test_image.png");
}