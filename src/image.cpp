#include <format>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "image.h"
#include "stb_image_write.h"

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

void Image::HorizontalFlip() {
    for (int y = 0; y < height; ++y) {
        int start = y * width * channels;
        int end = ((y + 1) * width - 1) * channels;
        while (start < end) {
            for (int i = 0; i < channels; ++i) {
                std::swap(pixels[start + i], pixels[end + i]);
            }
            start += channels;
            end -= channels;
        }
    }
};

void Image::RotateRight() {
    Transpose();
    HorizontalFlip();
};

void Image::Save(const std::filesystem::path& path) {
    auto ext = path.extension();
    int exitCode = 1;
    if (ext == ".jpg") {
        exitCode = stbi_write_jpg(path.c_str(), width, height, channels,
                                  pixels.get(), 90);
    } else if (ext == ".png") {
        exitCode = stbi_write_png(path.c_str(), width, height, channels,
                                  pixels.get(), width * channels);
    } else {
        throw std::runtime_error(
            std::format("{} files are unsupported", ext.string()));
    }

    if (!exitCode) {
        throw std::runtime_error("failed to write file");
    }
};
