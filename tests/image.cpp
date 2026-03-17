#include "image.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

Image constructImage(int height, int width, int channels = 3) {
    std::unique_ptr<stbi_uc[], decltype(&stbi_image_free)> pixels{
        static_cast<stbi_uc*>(malloc(height * width * channels)),
        stbi_image_free};
    for (int i = 0; i < height * width * channels; ++i) {
        pixels[i] = i;
    }
    Image image(std::move(pixels), height, width, channels);
    return image;
}

TEST_CASE("Image pixel aggregation", "[image]") {
    int height = 2, width = 2;
    Image image = constructImage(height, width);
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
        int height = 2, width = 2;
        Image image = constructImage(height, width);
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
        int height = 2, width = 3;
        Image image = constructImage(height, width);
        image.Transpose();
        std::vector<std::vector<stbi_uc>> expected = {{0, 1, 2}, {9, 10, 11},
                                                      {3, 4, 5}, {12, 13, 14},
                                                      {6, 7, 8}, {15, 16, 17}};
        REQUIRE(image.Pixels() == expected);
        REQUIRE(image.Height() == width);
        REQUIRE(image.Width() == height);
    }
}

TEST_CASE("Image Flip", "[image]") {
    SECTION("Horizontal Flip") {
        int height = 3, width = 2;
        Image image = constructImage(height, width);
        image.HorizontalFlip();
        std::vector<std::vector<stbi_uc>> expected = {
            {3, 4, 5}, {0, 1, 2},    {9, 10, 11},
            {6, 7, 8}, {15, 16, 17}, {12, 13, 14},
        };

        REQUIRE(image.Pixels() == expected);
    }
}

TEST_CASE("Blur", "[!benchmark]") {
    Image image(TEST_FIXTURES_DIR "/large.jpg");
    BENCHMARK_ADVANCED("Box Blur - 16 kernel size - 1 Pass")(
        Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.BoxBlur(16, 1); });
    };

    BENCHMARK_ADVANCED("Box Blur - 32 kernel size - 1 Pass")(
        Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.BoxBlur(32, 1); });
    };

    BENCHMARK_ADVANCED("Gaussian Blur - 16 kernel size - 5 sigma")
    (Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.GaussianBlur(16, 5); });
    };

    BENCHMARK_ADVANCED("Gaussian Blur - 32 kernel size - 5 sigma")
    (Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.GaussianBlur(32, 5); });
    };
}
