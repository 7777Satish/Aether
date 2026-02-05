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
