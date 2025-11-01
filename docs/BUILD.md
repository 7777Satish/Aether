# Build Documentation

This document describes the build system, dependencies, and compilation process for Aether.

## Build System Overview

Aether uses a traditional Makefile-based build system. The build process compiles C source files into object files and links them with required libraries.

## Makefile Structure

```makefile
# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TARGET = $(BUILDDIR)/main

# Build targets
all: $(TARGET)
clean: 
	rm -rf $(BUILDDIR)/*
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/codeeditor
```

## Dependencies

### Required Libraries

#### SDL2 (Simple DirectMedia Layer)
- **Purpose**: Graphics rendering, window management, event handling
- **Version**: 2.0.0 or later
- **Ubuntu/Debian**: `libsdl2-dev`
- **Fedora/RHEL**: `SDL2-devel`
- **Arch**: `sdl2`
- **macOS**: `brew install sdl2`

#### SDL2_image
- **Purpose**: Image loading (PNG, JPG support)
- **Ubuntu/Debian**: `libsdl2-image-dev`
- **Fedora/RHEL**: `SDL2_image-devel`
- **Arch**: `sdl2_image`
- **macOS**: `brew install sdl2_image`

#### SDL2_ttf
- **Purpose**: TrueType font rendering
- **Ubuntu/Debian**: `libsdl2-ttf-dev`
- **Fedora/RHEL**: `SDL2_ttf-devel`
- **Arch**: `sdl2_ttf`
- **macOS**: `brew install sdl2_ttf`

#### GTK3
- **Purpose**: Native file dialogs
- **Ubuntu/Debian**: `libgtk-3-dev`
- **Fedora/RHEL**: `gtk3-devel`
- **Arch**: `gtk3`
- **macOS**: `brew install gtk+3`

#### pthread
- **Purpose**: Threading support for non-blocking file operations
- **Note**: Usually included with system libc

### Build Tools

#### GCC or Clang
- **Purpose**: C compiler
- **Ubuntu/Debian**: `gcc` or `clang`
- **Fedora/RHEL**: `gcc` or `clang`
- **Arch**: `gcc` or `clang`
- **macOS**: Xcode command line tools

#### Make
- **Purpose**: Build automation
- **Ubuntu/Debian**: `make`
- **Fedora/RHEL**: `make`
- **Arch**: `make`
- **macOS**: Included with Xcode tools

#### pkg-config
- **Purpose**: Library configuration management
- **Ubuntu/Debian**: `pkg-config`
- **Fedora/RHEL**: `pkgconfig`
- **Arch**: `pkgconf`
- **macOS**: `brew install pkg-config`

## Compilation Process

### Step 1: Object File Generation
Each C source file is compiled to an object file:
```bash
gcc -Iinclude -Wall -Wextra -g `sdl2-config --cflags` -c src/main.c -o build/main.o
gcc -Iinclude -Wall -Wextra -g `sdl2-config --cflags` -c src/renderer.c -o build/renderer.o
# ... and so on for each source file
```

### Step 2: Linking
All object files are linked together with libraries:
```bash
gcc build/*.o -o build/main `sdl2-config --libs` -lSDL2_image -lSDL2_ttf
```

## Build Targets

### `make` or `make all`
Builds the complete project:
```bash
make
```
- Creates build directory if it doesn't exist
- Compiles all source files to object files
- Links object files to create executable
- Places executable at `build/main`

### `make clean`
Removes all build artifacts:
```bash
make clean
```
- Deletes all object files
- Removes executable
- Keeps source files and assets

### `make debug`
Builds with additional debug information:
```bash
make debug
```
- Includes debug symbols (-g flag)
- Disables optimizations
- Enables additional warnings

### `make install`
Installs the editor system-wide:
```bash
sudo make install
```
- Copies executable to `/usr/local/bin/`
- Creates desktop entry
- Sets up file associations

### `make uninstall`
Removes system-wide installation:
```bash
sudo make uninstall
```

## Cross-Platform Build Notes

### Linux
- Standard build process works on all major distributions
- May need to install development packages (`-dev` or `-devel`)
- Some distributions require `libpthread` explicitly

### macOS
- Requires Xcode command line tools
- Use Homebrew for dependency management
- May need to set `CPATH` and `LIBRARY_PATH` environment variables

### Windows (MinGW/MSYS2)
- Use MSYS2 for Unix-like build environment
- Install dependencies via pacman:
  ```bash
  pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf
  ```
- May need to adjust Makefile for Windows paths

## Environment Variables

### SDL2_CONFIG
Path to sdl2-config script:
```bash
export SDL2_CONFIG=/usr/local/bin/sdl2-config
```

### CC
Specify different compiler:
```bash
CC=clang make
```

### CFLAGS
Additional compiler flags:
```bash
CFLAGS="-O3 -DNDEBUG" make
```

## Troubleshooting

### Common Build Errors

#### "SDL.h: No such file or directory"
- **Cause**: SDL2 development headers not installed
- **Solution**: Install `libsdl2-dev` package

#### "undefined reference to SDL_Init"
- **Cause**: SDL2 library not linked properly
- **Solution**: Check `sdl2-config --libs` output

#### "zenity: command not found"
- **Cause**: Zenity not installed (for file dialogs)
- **Solution**: Install zenity package or modify file dialog code

#### "make: command not found"
- **Cause**: Make not installed
- **Solution**: Install build-essential package

#### Permission denied errors
- **Cause**: No write permission to build directory
- **Solution**: Ensure proper directory permissions

### Debug Build
For debugging, compile with additional flags:
```bash
CFLAGS="-g -O0 -DDEBUG" make
```

### Performance Build
For optimized release build:
```bash
CFLAGS="-O3 -DNDEBUG" make
```

## Build Optimization

### Parallel Builds
Use multiple CPU cores:
```bash
make -j$(nproc)
```

### Link-Time Optimization (LTO)
Enable LTO for better performance:
```bash
CFLAGS="-flto" LDFLAGS="-flto" make
```

### Static Linking
Create self-contained executable:
```bash
LDFLAGS="-static" make
```
Note: May require static versions of libraries

## Continuous Integration

### GitHub Actions Example
```yaml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Install dependencies
      run: |
        sudo apt update
        sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libgtk-3-dev
    - name: Build
      run: make
    - name: Test
      run: ./build/main --version
```

## Advanced Build Configuration

### Custom Install Prefix
```bash
PREFIX=/opt/codeeditor make install
```

### Separate Debug and Release Builds
```bash
# Debug build
make BUILD_TYPE=debug

# Release build  
make BUILD_TYPE=release
```

### Cross-Compilation
```bash
# For ARM64
CC=aarch64-linux-gnu-gcc make

# For Windows (from Linux)
CC=x86_64-w64-mingw32-gcc make
```
