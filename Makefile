# Compiler
CC := gcc

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

# File patterns
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
EXECUTABLE := gulag

# Compiler flags
CFLAGS := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/stats -Wall -DCL_TARGET_OPENCL_VERSION=300
LDFLAGS := -lOpenCL -lm
OPT_FLAGS := -O3 -march=native
DEBUG_FLAGS := -g -fsanitize=address

# Default target to build the optimized version
.PHONY: all
all: directories $(EXECUTABLE)

# Create all necessary directories
DIRECTORIES := $(sort $(dir $(OBJECTS)))

.PHONY: directories
directories:
	@for dir in $(DIRECTORIES); do \
		mkdir -p "$$dir"; \
	done

# Link object files into the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@ $(OPT_FLAGS)

# Pattern rule for compiling source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c $< -o $@

# Target for debugging version with AddressSanitizer
.PHONY: debug
debug:
	$(MAKE) all CFLAGS="$(CFLAGS) $(DEBUG_FLAGS)" LDFLAGS="$(LDFLAGS) $(DEBUG_FLAGS)"

# Clean up build files and directories
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
