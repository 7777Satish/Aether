# Compiler
CC = gcc

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Output binary
TARGET = $(BUILD_DIR)/main

# Compiler flags
CFLAGS = -I$(INC_DIR) -Wall -Wextra -g `pkg-config --cflags sdl3 sdl3-image sdl3-ttf`
LDFLAGS = `pkg-config --libs sdl3 sdl3-image sdl3-ttf` -lm

# Default target
all: $(TARGET)

# Link objects into binary
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
