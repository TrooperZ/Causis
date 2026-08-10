BUILD_DIR ?= build
JOBS ?= 14
FILE ?= examples/program.au

.PHONY: configure build test run

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

build: configure
	cmake --build $(BUILD_DIR) -j $(JOBS)

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

run: build
	$(BUILD_DIR)/causis $(FILE)
