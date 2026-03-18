.PHONY: build test

build:
	cmake --build build

test: build
	ctest --test-dir build --output-on-failure -R tests

benchmark: build
	./build/tests/tests "[!benchmark]"
