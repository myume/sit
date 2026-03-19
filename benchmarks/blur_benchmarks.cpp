#include "image.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Box Blur Benchmarks", "[!benchmark][box]") {
    Image image(TEST_FIXTURES_DIR "/large.jpg");

    BENCHMARK_ADVANCED("Box Blur - 16 kernel size - 1 Pass")(
        Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.BoxBlur(16, 1); });
    };

    BENCHMARK_ADVANCED("Box Blur - 32 kernel size - 1 Pass")(
        Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.BoxBlur(32, 1); });
    };

    BENCHMARK_ADVANCED("Box Blur - 64 kernel size - 2 Pass")(
        Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.BoxBlur(64, 2); });
    };
}

TEST_CASE("Guassian Blur Benchmarks", "[!benchmark][guassian]") {
    Image image(TEST_FIXTURES_DIR "/large.jpg");

    BENCHMARK_ADVANCED("Gaussian Blur - 16 kernel size - 5 sigma")
    (Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.GaussianBlur(16, 5); });
    };

    BENCHMARK_ADVANCED("Gaussian Blur - 32 kernel size - 5 sigma")
    (Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.GaussianBlur(32, 5); });
    };

    BENCHMARK_ADVANCED("Gaussian Blur - 64 kernel size - 10 sigma")
    (Catch::Benchmark::Chronometer meter) {
        meter.measure([image]() mutable { return image.GaussianBlur(64, 10); });
    };
}

TEST_CASE("Kawase Blur Benchmarks", "[!benchmark][kawase]") {
    Image image(TEST_FIXTURES_DIR "/large.jpg");

    for (int passes : {2, 4, 8, 16, 32}) {
        BENCHMARK_ADVANCED(
            ("Kawase Blur - " + std::to_string(passes) + " Pass").c_str())(
            Catch::Benchmark::Chronometer meter) {
            meter.measure(
                [image, passes]() mutable { return image.KawaseBlur(passes); });
        };
    }
}
