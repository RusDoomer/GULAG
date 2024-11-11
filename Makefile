# Compiler
CC := gcc

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
TEST_DIR := tests

# File patterns
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
EXECUTABLE := gulag
TEST_EXECUTABLE := test_main

# Compiler flags
CFLAGS := -I$(INCLUDE_DIR) -Wall
OPT_FLAGS := -O3 -march=native
DEBUG_FLAGS := -g -fsanitize=address

# Target to build the optimized version
.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@ $(OPT_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c $< -o $@

# Target for debugging version with AddressSanitizer
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(DEBUG_FLAGS)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build files
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
