

#include "core/Encoder.h"

int main() {

    Split::ImageEncoder::encode("image.jpg", "image_edited.jpg", "diff");

    return 0;
}
