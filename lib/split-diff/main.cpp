

#include "core/Encoder.h"

int main() {

    Split::ImageEncoder::encode("image.png", "Brightness/image_bright.png", "Brightness/diff");
    Split::ImageEncoder::decode("image.png", "Brightness/diff", "Brightness/reconstructed.png");

    Split::ImageEncoder::encode("image.png", "Cold/image_cold.png", "Cold/diff");
    Split::ImageEncoder::decode("image.png", "Cold/diff", "Cold/reconstructed.png");

    Split::ImageEncoder::encode("image.png", "HandDraw/image_draw.png", "HandDraw/diff");
    Split::ImageEncoder::decode("image.png", "HandDraw/diff", "HandDraw/reconstructed.png");

    Split::ImageEncoder::encode("image.png", "Saturation/image_saturated.png", "Saturation/diff");
    Split::ImageEncoder::decode("image.png", "Saturation/diff", "Saturation/reconstructed.png");

    Split::ImageEncoder::encode("image.png", "Tint/image_greened.png", "Tint/diff");
    Split::ImageEncoder::decode("image.png", "Tint/diff", "Tint/reconstructed.png");

    Split::ImageEncoder::encode("image.png", "Scraps/image_scraps.png", "Scraps/diff");
    Split::ImageEncoder::decode("image.png", "Scraps/diff", "Scraps/reconstructed.png");

    return 0;
}
