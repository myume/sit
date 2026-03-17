#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"
#include "stb_image_write.h"

#include <algorithm>
#include <cstdlib>
#include <format>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

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
                // output will have garbage in it, but we'll be throwing away
                // the old pixels after so it's fine.
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

template <typename Fn>
std::vector<float> Image::sampleSquare(int x, int y, int size, Fn weightFn) {
    std::vector<float> vals(channels);

    int yStart = std::max(y - size, 0);
    int yEnd = std::min(y + size, height - 1);
    int xStart = std::max(x - size, 0);
    int xEnd = std::min(x + size, width - 1);

    // accumulate the pixel values
    float weights = 0.0;
    for (int i = yStart; i <= yEnd; ++i) {
        for (int j = xStart; j <= xEnd; ++j) {
            float w = weightFn(j, i);
            weights += w;
            for (int k = 0; k < channels; ++k) {
                vals[k] += w * pixels[(i * width + j) * channels + k];
            }
        }
    }

    // normalize the values
    for (int i = 0; i < vals.size(); ++i) {
        vals[i] = vals[i] / weights;
    }

    return vals;
};

void Image::BoxBlur(int size, int passes) {
    for (int pass = 0; pass < passes; ++pass) {
        stbi_uc* output =
            static_cast<stbi_uc*>(malloc(height * width * channels));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::vector<float> sample =
                    sampleSquare(x, y, size, [](int x, int y) { return 1; });
                for (int i = 0; i < channels; ++i) {
                    output[(y * width + x) * channels + i] = sample[i];
                }
            }
        }
        pixels.reset(output);
    }
};

inline float gaussian(float x, float y, float sigma) {
    return exp(-(x * x + y * y) / (2 * sigma * sigma));
}

void Image::GaussianBlur(int size, float sigma) {
    stbi_uc* output = static_cast<stbi_uc*>(malloc(height * width * channels));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::vector<float> sample =
                sampleSquare(x, y, size, [&x, &y, &sigma](int i, int j) {
                    return gaussian(x - i, y - j, sigma);
                });
            for (int i = 0; i < channels; ++i) {
                output[(y * width + x) * channels + i] = sample[i];
            }
        }
    }
    pixels.reset(output);
};
