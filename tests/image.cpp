#include "image.h"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Image pixel aggregation", "[image]") {
    int height = 2, width = 2, channels = 3;
    std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels{
        static_cast<stbi_uc*>(malloc(height * width * channels)),
        stbi_image_free};

    for (int i = 0; i < height * width * channels; ++i) {
        pixels[i] = i;
    }
    Image image(std::move(pixels), height, width, channels);
    std::vector<std::vector<stbi_uc>> expected = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {9, 10, 11},
    };
    REQUIRE(image.Pixels() == expected);
}

TEST_CASE("Image Tranposition", "[image]") {
    SECTION("Square Images") {
        int height = 2, width = 2, channels = 3;
        std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels{
            static_cast<stbi_uc*>(malloc(height * width * channels)),
            stbi_image_free};

        for (int i = 0; i < height * width * channels; ++i) {
            pixels[i] = i;
        }

        Image image(std::move(pixels), height, width, channels);
        image.Transpose();
        std::vector<std::vector<stbi_uc>> expected = {
            {0, 1, 2},
            {6, 7, 8},
            {3, 4, 5},
            {9, 10, 11},
        };
        REQUIRE(image.Pixels() == expected);
    }

    SECTION("Rectangle Images") {
        int height = 2, width = 3, channels = 3;
        std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels{
            static_cast<stbi_uc*>(malloc(height * width * channels)),
            stbi_image_free};

        for (int i = 0; i < height * width * channels; ++i) {
            pixels[i] = i;
        }

        Image image(std::move(pixels), height, width, channels);
        image.Transpose();
        std::vector<std::vector<stbi_uc>> expected = {{0, 1, 2}, {9, 10, 11},
                                                      {3, 4, 5}, {12, 13, 14},
                                                      {6, 7, 8}, {15, 16, 17}};
        REQUIRE(image.Pixels() == expected);
        REQUIRE(image.Height() == width);
        REQUIRE(image.Width() == height);
    }
}
