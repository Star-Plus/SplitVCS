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
    auto result = encoder.encode(rootPath+"/Sample.psd", rootPath+"/encoded");

}

TEST(PsdEncoderTest, EncodeDelta)
{
    const std::string rootPath = "psd_enc/";

    Split::PsdEncoder encoder;
    std::string outPath = rootPath + "encoded2";
    std::stack<std::string> deltas = {};
    encoder.encode(rootPath + "encoded", deltas, rootPath+"Sample.psd", outPath);
}

TEST(PsdEncoderTest, EncodeTwice)
{
    const std::string rootPath = "psd_enc/";

    Split::PsdEncoder encoder;

    std::string outPath = rootPath + "encoded3";
    std::stack<std::string> deltas = {};
    deltas.push(rootPath + "encoded2");

    encoder.encode(rootPath + "encoded", deltas, rootPath+"Sample.psd", outPath);
}