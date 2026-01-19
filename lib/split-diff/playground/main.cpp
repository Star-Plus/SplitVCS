#include <iostream>
#include <fstream>
#include "components/DeltaCompressor.h"

int main() {

    std::ifstream fileA("OUR_CEO.png", std::ios::binary);
    if (!fileA) {
        std::cerr << "Failed to open OUR_CEO.png\n";
        return 1;
    }

    std::ifstream fileB("OUR_INTERN.png", std::ios::binary);
    if (!fileB) {
        std::cerr << "Failed to open OUR_INTERN.png\n";
        return 1;
    }

    std::ofstream outputFile("image_diff.png", std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to create output file image_diff.png\n";
        return 1;
    }

    Split::DeltaCompressor compressor;
    compressor.encode(
        Split::Blob(fileA, "OUR_CEO.png"), 
        Split::Blob(fileB, "OUR_INTERN.png"), 
        Split::Blob(outputFile, "image_diff.png")
    );

    return 0;
}