# Build Documentation

This document describes the build system, dependencies, and compilation process for Aether (CodeEditor).

## Build System Overview

Aether uses CMake as its build system. The build process configures the project, finds dependencies, compiles C source files into object files, and links them with required libraries.

## CMake Structure

The project root contains a `CMakeLists.txt` file which defines the build process:

```cmake
cmake_minimum_required(VERSION 3.16)

project(CodeEditor C)

# Use modern C
set(CMAKE_C_STANDARD 11)

# Find SDL3 packages installed via pkg-config
find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL3 REQUIRED sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED sdl3-image)
pkg_check_modules(SDL3_TTF REQUIRED sdl3-ttf)

# Include directories
include_directories(
    include
    ${SDL3_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
    ${SDL3_TTF_INCLUDE_DIRS}
)

# Collect source files
file(GLOB SRC_FILES src/*.c)

# Create executable
add_executable(editor ${SRC_FILES})

# Link libraries
target_link_libraries(editor
    ${SDL3_LIBRARIES}
    ${SDL3_IMAGE_LIBRARIES}
    ${SDL3_TTF_LIBRARIES}
    m
)
```

## Dependencies

### Required Libraries

#### SDL3 (Simple DirectMedia Layer)
- **Purpose**: Graphics rendering, window management, event handling
- **Version**: 3.0.0 or later
- **Installation**: Use package manager or build from source (see below).

#### SDL3_image
- **Purpose**: Image loading (PNG, JPG support)
- **Extension**: A separate library that works with SDL3.

#### SDL3_ttf
- **Purpose**: TrueType font rendering.
- **Extension**: A separate library that works with SDL3.

### Build Tools
- **CMake**: Cross-platform build system generator.
- **Make** (or generic build tool like Ninja): To run the generated build files.
- **GCC/Clang/MSVC**: C compiler supporting C11 standard.
- **pkg-config**: Helper tool used to find installed libraries.

## Building Instructions

### Linux

1.  **Install CMake & Build Tools**:
    ```bash
    sudo apt update
    sudo apt install build-essential cmake pkg-config
    ```

2.  **Install SDL3 Dependencies**:
    Since SDL3 is relatively new, it might not be in all distribution repositories yet. Check your package manager or build from source.

    *Example (if available via package manager):*
    ```bash
    sudo apt install libsdl3-dev libsdl3-image-dev libsdl3-ttf-dev
    ```

    *Building SDL3 from source (recommended if not in repos):*
    Clone the SDL3 repository, create a build directory, run cmake, and install. Repeat for SDL3_image and SDL3_ttf.

3.  **Build Project**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4.  **Run**:
    ```bash
    ./editor
    ```

### Windows

1.  Ensure you have a C compiler installed (MinGW or Visual Studio).
2.  Install CMake.
3.  Download the SDL3 development libraries for Windows (VC or MinGW versions).
4.  Configure the project using CMake GUI or CLI, pointing `CMAKE_PREFIX_PATH` to your SDL3 installation if necessary.
5.  Open the generated project (e.g., Visual Studio Solution) and build.

### macOS

1.  Install dependencies via Homebrew:
    ```bash
    brew install cmake sdl3 sdl3_image sdl3_ttf
    ```
2.  Build:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

## Build Optimization and Types

### Release Build
To build for performance (optimized):
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### Debug Build
To build for debugging (symbols enabled):
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Troubleshooting

### "Could not find PkgConfig"
Ensure `pkg-config` is installed.

### "Could not find ... (missing: ...)"
CMake couldn't locate the SDL3 libraries.
- Ensure they are installed.
- On Windows, ensure `CMAKE_PREFIX_PATH` includes the path to SDL3.
- On Linux, ensure `.pc` files are in `PKG_CONFIG_PATH`.

### Permission issues
If `make install` fails, run with `sudo`.

