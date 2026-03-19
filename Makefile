.PHONY: build test benchmark

build:
	cmake --build build

test: build
	ctest --test-dir build --output-on-failure -R tests

benchmark: build
	./build/bin/benchmark "[!benchmark]"
