# Compiler
CC := gcc

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
TEST_DIR := tests

# File patterns
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SOURCES)))
EXECUTABLE := gulag

# Compiler flags
CFLAGS := -I$(INCLUDE_DIR) -Wall
LDFLAGS := 
OPT_FLAGS := -O3 -march=native
DEBUG_FLAGS := -g -fsanitize=address

# Default target to build the optimized version
.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILD_DIR)
    $(CC) $^ $(LDFLAGS) -o $@ $(OPT_FLAGS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
    $(CC) $(CFLAGS) $(OPT_FLAGS) -c $< -o $@

# Target for debugging version with AddressSanitizer
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: LDFLAGS += $(DEBUG_FLAGS)
debug: $(EXECUTABLE)

# Create build directory if it doesn't exist
$(BUILD_DIR):
    mkdir -p $(BUILD_DIR)

# Clean up build files
.PHONY: clean
clean:
    rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Additional target for documentation or tests
.PHONY: test
test:
    @echo "Running tests..."
    # Add commands to run your tests here

.PHONY: docs
docs:
    @echo "Generating documentation..."
    # Add commands to generate documentation here
