#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../AssetDissolver.h"
#include "../AssetBlockBuilder.h"

namespace fs = std::filesystem;

TEST(AssetDissolveTest, BasicFile)
{
    std::string rootPath = "test_dissolve/";

    Split::OffsetBound offsetBound(20, 20);

    Split::AssetDissolver dissolver;
    dissolver.slice(rootPath + "file", offsetBound, rootPath + "unit1");

    std::set offsets = {offsetBound};
    dissolver.dissolve(rootPath + "file", rootPath + "base",  offsets);

    Split::AssetBlockBuilder combiner;

    std::ifstream inFile(rootPath + "unit1");

    Split::BlockUnit block(std::make_shared<std::ifstream>(rootPath + "unit1"), offsetBound);
    std::set blocks = {block};

    combiner.combine(rootPath+"base", blocks, rootPath + "decoded");
}

TEST(AssetDissolveTest, MultipleSlices)
{
    std::string rootPath = "test_dissolve/";

    Split::OffsetBound offsetBound1(10, 10);
    Split::OffsetBound offsetBound2(20, 10);
    Split::OffsetBound offsetBound3(40, 10);

    Split::AssetDissolver dissolver;
    dissolver.slice(rootPath + "file", offsetBound1, rootPath + "unit1");
    dissolver.slice(rootPath + "file", offsetBound2, rootPath + "unit2");
    dissolver.slice(rootPath + "file", offsetBound3, rootPath + "unit3");


    std::set offsets = {offsetBound1, offsetBound2, offsetBound3};
    dissolver.dissolve(rootPath + "file", rootPath + "base",  offsets);

    Split::AssetBlockBuilder combiner;

    Split::BlockUnit block1(std::make_shared<std::ifstream>(rootPath + "unit1"), offsetBound1);
    Split::BlockUnit block2(std::make_shared<std::ifstream>(rootPath + "unit2"), offsetBound2);
    Split::BlockUnit block3(std::make_shared<std::ifstream>(rootPath + "unit3"), offsetBound3);

    std::set blocks = {block1, block2, block3};

    combiner.combine(rootPath+"base", blocks, rootPath + "decoded");
}