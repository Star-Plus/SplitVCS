#include <iostream>

#include "LiveEncoder.h"
#include "features/ImageCompressor.h"

int main() {
    std::cout << "Welcome to the SplitLive Playground!" << std::endl;

    const Split::LiveEncoder le;
    le.encode("image.jpg", "compressed_image.webp", {10});

    return 0;
}