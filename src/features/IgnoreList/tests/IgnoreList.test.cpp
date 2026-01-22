//
// Created by Ahmed Mustafa on 1/22/2026.
//

#include <gtest/gtest.h>

#include "core/Alias.h"
#include "features/IgnoreList/IgnoreList.h"

TEST(IgnoreListTest, SimpleCase)
{
    const str rootPath = "test_repo";

    Split::IgnoreList il(rootPath);

    il.add("Hello");

    bool isIgnored = il.isIgnored("Hello/file.txt");
    ASSERT_TRUE(isIgnored);
}