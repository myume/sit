#pragma once
#include "stb_image.h"

#include <filesystem>
#include <vector>

class Image {
  public:
    explicit Image(const std::filesystem::path& path);

    explicit Image(
        std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels,
        int height, int width, int channels);

    int Height() const;

    int Width() const;

    int Channels() const;

    std::vector<std::vector<stbi_uc>> Pixels() const;

    void Transpose();

    void HorizontalFlip();

  private:
    int height, width;
    int channels;

    std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels;
};
