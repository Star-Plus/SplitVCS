//
// Created by Ahmed Mustafa on 2/3/2026.
//

#include <gtest/gtest.h>

#include "features/photoshop/PsdUltraviolet.h"

TEST(PsdUltravioletTest, SimpleCase)
{
    const std::string rootPath = "test_uv_psd/";
    Split::PsdUltraviolet uv;
    uv.encode(rootPath+"Sample.psd", rootPath+"Sample");
}