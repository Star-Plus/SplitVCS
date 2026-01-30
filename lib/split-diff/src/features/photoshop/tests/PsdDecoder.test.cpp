//
// Created by Ahmed Mustafa on 1/30/2026.
//

#include <gtest/gtest.h>

#include "features/photoshop/PsdDecoder.h"

TEST(PsdDecoderTest, DecodeBase)
{
    const std::string rootPath = "psd_enc";
    Split::PsdDecoder decoder;

    std::stack<std::string> st;
    std::string out = rootPath + "/decoded.psd";
    decoder.decode(rootPath + "/encoded", st, out);
}