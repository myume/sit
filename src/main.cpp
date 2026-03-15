#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <print>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::println(stderr, "Usage: sit <image_path>");
        return -1;
    }

    int width, height, channels;
    stbi_uc *pixels = stbi_load(argv[1], &width, &height, &channels, 0);

    std::println("{} {}", width, height);
}
