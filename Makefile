.PHONY: build test

build:
	cmake --build build

test: build
	ctest --test-dir build
