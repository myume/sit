#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <filesystem>

#include "stb_image.h"

class Image {
  public:
    explicit Image(const std::filesystem::path& path);

    Image(const Image& image);

    Image& operator=(const Image& image);
    Image& operator=(Image&& image);

    Image(Image&& image) noexcept;

    ~Image();

    int Height() const;

    int Width() const;

  private:
    int height, width;
    int channels;

    stbi_uc* pixels;
};
