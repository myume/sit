#include <print>

#include "image.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::println(stderr, "Usage: sit <image_path>");
        return -1;
    }

    Image image(argv[1]);

    std::println("Loaded image: {}x{}", image.Width(), image.Height());

    std::println("{}", image.Pixels()[0]);
}
