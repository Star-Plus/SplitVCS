//
// Created by Ahmed Mustafa on 1/30/2026.
//

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "features/photoshop/PsdEncoder.h"

TEST(PsdEncoderTest, EncodeBase)
{
    const std::string rootPath = "psd_enc";

    std::fstream psdFile(rootPath+"/Sample.psd", std::ios::in);
    ASSERT_TRUE(psdFile.good());
    psdFile.close();

    Split::PsdEncoder encoder;
    auto result = encoder.encode(rootPath+"/Sample.psd", rootPath+"/encoded.psd");

}