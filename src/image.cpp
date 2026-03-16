#define STB_IMAGE_IMPLEMENTATION

#include "image.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <utility>

Image::Image(const std::filesystem::path& path) {
    pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!pixels) {
        throw std::runtime_error("Failed to load image");
    }
};

Image::~Image() { stbi_image_free(pixels); };

Image::Image(const Image& other)
    : width(other.width), height(other.height), channels(other.channels) {
    size_t size = height * width * channels;
    pixels = static_cast<stbi_uc*>(malloc(size));
    if (!pixels) {
        throw std::bad_alloc();
    }
    std::memcpy(pixels, other.pixels, size);
};

Image& Image::operator=(const Image& other) {
    if (this == &other) {
        return *this;
    }

    width = other.width;
    height = other.height;
    channels = other.channels;

    stbi_image_free(pixels);
    size_t size = height * width * channels;
    pixels = static_cast<stbi_uc*>(malloc(size));
    if (!pixels) {
        throw std::bad_alloc();
    }
    std::memcpy(pixels, other.pixels, size);

    return *this;
};

Image::Image(Image&& other) noexcept
    : pixels(std::exchange(other.pixels, nullptr)),
      width(std::exchange(other.width, 0)),
      height(std::exchange(other.height, 0)),
      channels(std::exchange(other.channels, 0)) {};

Image& Image::operator=(Image&& other) {
    if (this == &other) {
        return *this;
    }
    stbi_image_free(pixels);
    pixels = std::exchange(other.pixels, nullptr);
    width = std::exchange(other.width, 0);
    height = std::exchange(other.height, 0);
    channels = std::exchange(other.channels, 0);
    return *this;
};

int Image::Width() const { return width; };

int Image::Height() const { return height; };

int Image::Channels() const { return channels; };

std::vector<std::vector<stbi_uc>> Image::Pixels() const {
    std::vector<std::vector<stbi_uc>> output;
    output.reserve(height * width);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::vector<stbi_uc> channel;
            for (int i = 0; i < channels; ++i) {
                channel.push_back(pixels[y * height + x + i]);
            }
            output.push_back(channel);
        }
    }
    return output;
};

void Image::Transpose() {
    stbi_uc* output =
        (height != width)
            ? static_cast<stbi_uc*>(malloc(height * width * channels))
            : pixels;

    for (int y = 0; y < height; ++y) {
        for (int x = (height == width) * (y + 1); x < width; ++x) {
            for (int i = 0; i < channels; i++) {
                pixels[y * width + x + i] = output[x * height + y + i];
            }
        }
    }

    std::swap(height, width);
    if (height != width) {
        stbi_image_free(pixels);
    }
    pixels = output;
};
