#define STB_IMAGE_IMPLEMENTATION
#include "image.h"

#include <stdexcept>
#include <utility>

Image::Image(const std::filesystem::path& path)
    : pixels(nullptr, stbi_image_free) {
    pixels.reset(stbi_load(path.c_str(), &width, &height, &channels, 0));
    if (!pixels) {
        throw std::runtime_error("Failed to load image");
    }
};

Image::Image(std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> img,
             int height, int width, int channels)
    : pixels(std::move(img)), height(height), width(width), channels(channels) {
}

int Image::Width() const { return width; };

int Image::Height() const { return height; };

int Image::Channels() const { return channels; };

std::vector<std::vector<stbi_uc>> Image::Pixels() const {
    std::vector<std::vector<stbi_uc>> output;
    output.reserve(height * width);
    for (int i = 0; i < height * width * channels; i += channels) {
        std::vector<stbi_uc> channel;
        for (int j = 0; j < channels; ++j) {
            channel.push_back(pixels[i + j]);
        }
        output.push_back(channel);
    }
    return output;
};

void Image::Transpose() {
    stbi_uc* output =
        (height != width)
            ? static_cast<stbi_uc*>(malloc(height * width * channels))
            : pixels.get();

    for (int y = 0; y < height; ++y) {
        for (int x = (height == width) * (y + 1); x < width; ++x) {
            for (int i = 0; i < channels; i++) {
                std::swap(output[(x * height + y) * channels + i],
                          pixels[(y * width + x) * channels + i]);
            }
        }
    }

    if (height != width) {
        std::swap(height, width);
        pixels.reset(output);
    }
};
