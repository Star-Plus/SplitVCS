//
// Created by Ahmed Mustafa on 1/29/2026.
//

#include <gtest/gtest.h>
#include <PsdSdk/PsdMallocAllocator.h>
#include <PsdSdk/PsdNativeFile.h>

TEST(PsdIntegrationTest, FirstCall)
{
    const std::wstring srcPath = L"PSDs/sample.psd";

    psd::MallocAllocator allocator;
    psd::NativeFile file(&allocator);

    ASSERT_TRUE(file.OpenRead(srcPath.c_str()));
}