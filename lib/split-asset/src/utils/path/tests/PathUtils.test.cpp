//
// Created by Ahmed Mustafa on 2/2/2026.
//

#include <gtest/gtest.h>

#include "utils/path/PathUtils.h"

TEST(PathUtilsTest, TestRemoveSuffix)
{
    std::string path = "Hello.txt";

    Split::PathUtils::removeSuffix(path, ".txt");

    ASSERT_EQ("Hello", path);
}