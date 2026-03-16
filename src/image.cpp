#include "image.h"

#include <cstddef>
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
