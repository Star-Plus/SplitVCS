#include <iostream>
#include <fstream>
#include "components/DeltaCompressor.h"

int main() {

    const auto Base = "image.jpg";
    const auto Modified = "image_edited.jpg";
    const auto Delta = "delta.webp";

    std::ifstream fileA(Base, std::ios::binary);
    if (!fileA) {
        std::cerr << "Failed to open " << Base << std::endl;
        return 1;
    }

    std::ifstream fileB(Modified, std::ios::binary);
    if (!fileB) {
        std::cerr << "Failed to open " << Modified << std::endl;
        return 1;
    }

    std::ofstream outputFile(Delta, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to create output " << Delta << std::endl;
        return 1;
    }

    Split::DeltaCompressor compressor;
    compressor.encode(
        Split::Blob(fileA, Base),
        Split::Blob(fileB, Modified),
        Split::Blob(outputFile, Delta)
    );

    std::ofstream constructedOutput("constructed.png", std::ios::binary);
    if (!constructedOutput) {
        std::cerr << "Failed to create constructed output file\n";
        return 1;
    }

    fileA.close();
    fileB.close();
    outputFile.close();

    std::ifstream baseFile(Base, std::ios::binary);
    if (!baseFile) {
        std::cerr << "Failed to open " << Base << std::endl;
    }

    std::ifstream diffFile(Delta, std::ios::binary);
    if (!diffFile) {
        std::cerr << "Failed to open " << Delta << std::endl;
        return 1;
    }

    compressor.decode(
        Split::Blob(baseFile, Base),
        Split::Blob(diffFile, Delta),
        Split::Blob(constructedOutput, "constructed.png")
    );

    return 0;
}