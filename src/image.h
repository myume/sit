#pragma once
#include "stb_image.h"

#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <vector>

class Image {
  public:
    explicit Image(const std::filesystem::path& path);

    Image(const Image& other);
    Image operator=(const Image& other);

    explicit Image(
        std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels,
        int height, int width, int channels);

    int Height() const;

    int Width() const;

    int Channels() const;

    std::vector<std::vector<stbi_uc>> Pixels() const;

    void Transpose();

    void HorizontalFlip();

    void RotateRight();

    void BoxBlur(int size, int passes);

    void GaussianBlur(int size, float sigma);

    void Save(const std::filesystem::path& path);

  private:
    int height, width;
    int channels;

    std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels;
};

enum class Transformation {
    Transpose,
    HorizontalFlip,
    RotateRight,
    BoxBlur,
    GaussianBlur,
};

inline Transformation parseTransformation(std::string_view s) {
    static const std::unordered_map<std::string_view, Transformation> mapping =
        {
            {"transpose", Transformation::Transpose},
            {"rotate_right", Transformation::RotateRight},
            {"horizontal_flip", Transformation::HorizontalFlip},
            {"box_blur", Transformation::BoxBlur},
            {"gaussian_blur", Transformation::GaussianBlur},
        };

    if (!mapping.contains(s)) {
        std::string message = std::format("Invalid transformation \"{}\"", s);
        message += "\n\nValid transformations:";
        for (const auto& [key, _] : mapping) {
            message += "\n - ";
            message += key;
        }
        throw std::runtime_error(message);
    }

    return mapping.at(s);
}
