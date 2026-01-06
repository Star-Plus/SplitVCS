#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utils/DeltaCompressor.h>
#include "lib/split-diff/core/Encoder.h"

int main() {

    const std::string testingFilters[] = {
        "Brightness",
        "Saturation",
        "Cold",
        "Scraps",
        "HandDraw",
        "Tint"
    };

    float compressionRatios[std::size(testingFilters)] = {0};

    for (int i = 0; i < std::size(testingFilters); i++) {
        const std::string& filter = testingFilters[i];
        std::cout << "Applying " << filter << " filter diff comparison:\n";

        const std::string originalPath = "image.png";
        const std::string editParentPath = filter + "/";
        const std::string editedPath = editParentPath + "edited.png";

        const std::fstream originalImage = std::fstream(originalPath, std::ios::in |  std::ios::binary);
        const std::fstream editedImage = std::fstream(editedPath, std::ios::in |  std::ios::binary);

        Split::DeltaCompressor deltaCompressor;

        std::ostringstream originalStream;
        originalStream << originalImage.rdbuf();
        std::ostringstream editedStream;
        editedStream << editedImage.rdbuf();


        const std::string delta = deltaCompressor.encode(originalStream.str(), editedStream.str());

        const auto byteDeltaSize = delta.size();

        std::cout << "Byte Delta size: " << byteDeltaSize / 1024 << "KB" << std::endl;

        Split::ImageEncoder::encode(originalPath, editedPath, editParentPath + "diff");

        std::fstream deltaStream = std::fstream(editParentPath + "diff_packed.webp", std::ios::in |  std::ios::binary);

        const auto packedDeltaSize = std::filesystem::file_size(editParentPath + "diff_packed.webp");

        std::cout << "Image Diff size: "<< packedDeltaSize / 1024 << "KB" << std::endl;

        const float compressionRatio = static_cast<float>(byteDeltaSize) / static_cast<float>(packedDeltaSize);

        compressionRatios[i] = compressionRatio;

        std::cout << "Saved by " << compressionRatio << " times" << "\n\n";
    }

    std::cout << "Summary of Compression Ratios:\n";
    float totalRatio = 0.0f;
    for (size_t i = 0; i < std::size(testingFilters); i++) {
        std::cout << testingFilters[i] << ": " << compressionRatios[i] << std::endl;
        totalRatio += compressionRatios[i];
    }

    std::sort(compressionRatios, compressionRatios + std::size(testingFilters));

    std::cout << "Average Compression Ratio: " << totalRatio / static_cast<float>(std::size(testingFilters)) << std::endl;
    std::cout << "Median Compression Ratio: " << compressionRatios[std::size(testingFilters) / 2] << std::endl;

    return 0;
}
