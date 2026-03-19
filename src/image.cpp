#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"
#include <stb_image_write.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
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
    : height(height), width(width), channels(channels), pixels(std::move(img)) {
}

Image::Image(const Image& other)
    : height(other.height), width(other.width), channels(other.channels),
      pixels(static_cast<stbi_uc*>(
                 malloc(other.height * other.width * other.channels)),
             stbi_image_free) {
    memcpy(pixels.get(), other.pixels.get(), height * width * channels);
};

Image& Image::operator=(const Image& other) {
    pixels.reset(static_cast<stbi_uc*>(
        malloc(other.height * other.width * other.channels)));
    height = other.height;
    width = other.width;
    channels = other.channels;
    memcpy(pixels.get(), other.pixels.get(), height * width * channels);
    return *this;
};

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

void Image::BoxBlur(int size, int passes) {
    size = size / 2;
    int total = height * width * channels;
    for (int pass = 0; pass < passes; ++pass) {
        stbi_uc* horizontal = static_cast<stbi_uc*>(malloc(total));

        // horizontal pass
        for (int y = 0; y < height; ++y) {
            int vals[4] = {};
            for (int i = 0; i < std::min(size, width); ++i) {
                for (int k = 0; k < channels; ++k) {
                    vals[k] += pixels[(y * width + i) * channels + k];
                }
            }

            for (int x = 0; x < width; ++x) {
                int xEnd = x + size;
                int xStart = x - size;
                int windowSize =
                    (std::min(xEnd, width - 1) - std::max(xStart, 0) + 1);

                for (int k = 0; k < channels; ++k) {
                    if (xEnd < width) {
                        vals[k] += pixels[(y * width + xEnd) * channels + k];
                    }

                    if (xStart - 1 >= 0) {
                        vals[k] -=
                            pixels[(y * width + xStart - 1) * channels + k];
                    }
                }

                for (int k = 0; k < channels; ++k) {
                    horizontal[(y * width + x) * channels + k] =
                        vals[k] / windowSize;
                }
            }
        }

        pixels.reset(horizontal);
        stbi_uc* vertical = static_cast<stbi_uc*>(malloc(total));

        // veritcal pass
        for (int x = 0; x < width; ++x) {
            int vals[4] = {};

            for (int j = 0; j < std::min(size, height); ++j) {
                for (int k = 0; k < channels; ++k) {
                    vals[k] += pixels[(j * width + x) * channels + k];
                }
            }

            for (int y = 0; y < height; ++y) {
                int yEnd = y + size;
                int yStart = y - size;
                int windowSize =
                    (std::min(yEnd, height - 1) - std::max(yStart, 0) + 1);

                for (int k = 0; k < channels; ++k) {
                    if (yEnd < height) {
                        vals[k] += pixels[(yEnd * width + x) * channels + k];
                    }
                    if (yStart - 1 >= 0) {
                        vals[k] -=
                            pixels[((yStart - 1) * width + x) * channels + k];
                    }
                }

                for (int k = 0; k < channels; ++k) {
                    vertical[(y * width + x) * channels + k] =
                        vals[k] / windowSize;
                }
            }
        }
        pixels.reset(vertical);
    }
};

inline float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2 * sigma * sigma));
}

void Image::GaussianBlur(int size, float sigma) {
    size = size / 2;
    int total = height * width * channels;
    stbi_uc* horizontal = static_cast<stbi_uc*>(malloc(total));

    std::vector<float> kernel(2 * size + 1);
    for (int i = -size; i <= size; ++i) {
        float weight = gaussian(i, sigma);
        kernel[i + size] = weight;
    }

    // horizontal pass
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float vals[4] = {};
            float weights = 0.0;
            for (int i = std::max(x - size, 0);
                 i <= std::min(x + size, width - 1); ++i) {
                float w = kernel[i - x + size];
                weights += w;
                for (int k = 0; k < channels; ++k) {
                    vals[k] += w * pixels[(y * width + i) * channels + k];
                }
            }

            for (int k = 0; k < channels; ++k) {
                horizontal[(y * width + x) * channels + k] = vals[k] / weights;
            }
        }
    }

    pixels.reset(horizontal);
    stbi_uc* vertical = static_cast<stbi_uc*>(malloc(total));

    // veritcal pass
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            float vals[4] = {};
            float weights = 0.0;
            for (int i = std::max(y - size, 0);
                 i <= std::min(y + size, height - 1); ++i) {
                float w = kernel[i - y + size];
                weights += w;
                for (int k = 0; k < channels; ++k) {
                    vals[k] += w * pixels[(i * width + x) * channels + k];
                }
            }

            for (int k = 0; k < channels; ++k) {
                vertical[(y * width + x) * channels + k] = vals[k] / weights;
            }
        }
    }
    pixels.reset(vertical);
};
