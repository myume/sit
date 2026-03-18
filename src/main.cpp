#include "CLI/CLI.hpp"
#include <exception>
#include <filesystem>
#include <print>

#include "image.h"

int main(int argc, char** argv) {
    CLI::App app{"Simple Image Transformer", "sit"};

    std::filesystem::path sourcePath;
    app.add_option("image_file", sourcePath, "Path to image")
        ->check(CLI::ExistingFile)
        ->required();

    std::filesystem::path output;
    app.add_option("-o,--output", output, "Path to output resulting image");

    std::vector<std::string> transformations;
    app.add_option("-t,--transform", transformations,
                   "Transformations to perform in order");

    int blurSize = 16;
    app.add_option("--blur-size", blurSize, "Blur size");

    int blurPasses = 1;
    app.add_option("--blur-passes", blurPasses, "Number of blur passes");

    float blurSigma = 3.0;
    app.add_option("--blur-sigma", blurSigma, "Sigma for gaussian blur");

    CLI11_PARSE(app, argc, argv);

    Image image(sourcePath);

    std::println("Loaded image: {} {}x{}", sourcePath.filename().string(),
                 image.Width(), image.Height());

    for (const auto& transform : transformations) {
        try {
            switch (parseTransformation(transform)) {
            case Transformation::Transpose:
                image.Transpose();
                break;
            case Transformation::HorizontalFlip:
                image.HorizontalFlip();
                break;
            case Transformation::RotateRight:
                image.RotateRight();
                break;
            case Transformation::BoxBlur:
                image.BoxBlur(blurSize, blurPasses);
                break;
            case Transformation::GaussianBlur:
                image.GaussianBlur(blurSize, blurSigma);
                break;
            }
        } catch (std::exception& e) {
            std::println(stderr, "{}", e.what());
            return -1;
        }
    }

    try {
        output = output.empty() ? sourcePath.filename() : output;
        std::println(stderr, "Saving image to {}", output.string());
        image.Save(output);
    } catch (std::exception& e) {
        std::println(stderr, "Failed to save image: {}", e.what());
        return -1;
    }
}
