#pragma once
#include "stb_image.h"

#include <filesystem>
#include <vector>

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

    int Channels() const;

    std::vector<std::vector<stbi_uc>> Pixels() const;

    void Transpose();

  private:
    int height, width;
    int channels;

    stbi_uc* pixels;
};
