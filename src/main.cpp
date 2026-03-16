#include "CLI/CLI.hpp"
#include <exception>
#include <print>

#include "image.h"

int main(int argc, char** argv) {
    CLI::App app{"Simple Image Transformer", "sit"};

    std::string filename;
    app.add_option("image_file", filename, "Path to image")
        ->check(CLI::ExistingFile)
        ->required();

    std::string output;
    app.add_option("-o,--output", output, "Path to output resulting image");

    CLI11_PARSE(app, argc, argv);

    Image image(filename);

    std::println("Loaded image: {}x{}", image.Width(), image.Height());

    try {
        image.Save(output.empty() ? filename : output);
    } catch (std::exception& e) {
        std::println("Failed to save image: {}", e.what());
        return -1;
    }
}
