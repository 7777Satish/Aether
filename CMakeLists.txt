cmake_minimum_required(VERSION 3.16)

project(CodeEditor C)

# Use modern C
set(CMAKE_C_STANDARD 11)

# Try to find SDL3 using standard Config mode (preferred for cross-platform)
find_package(SDL3 CONFIG)
find_package(SDL3_image CONFIG)
find_package(SDL3_ttf CONFIG)

if(SDL3_FOUND AND SDL3_image_FOUND AND SDL3_ttf_FOUND)
    message(STATUS "Found SDL3 via Config mode")
    set(SDL3_LIBRARIES SDL3::SDL3)
    set(SDL3_IMAGE_LIBRARIES SDL3_image::SDL3_image)
    set(SDL3_TTF_LIBRARIES SDL3_ttf::SDL3_ttf)
else()
    # Fallback to PkgConfig (common on Linux/Unix)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(SDL3 REQUIRED sdl3)
    pkg_check_modules(SDL3_IMAGE REQUIRED sdl3-image)
    pkg_check_modules(SDL3_TTF REQUIRED sdl3-ttf)

    # Include directories (only needed for PkgConfig path)
    include_directories(
        ${SDL3_INCLUDE_DIRS}
        ${SDL3_IMAGE_INCLUDE_DIRS}
        ${SDL3_TTF_INCLUDE_DIRS}
    )
endif()

include_directories(include)

# Collect source files
file(GLOB SRC_FILES src/*.c)

# Create executable
add_executable(editor ${SRC_FILES})

# Link libraries
target_link_libraries(editor
    ${SDL3_LIBRARIES}
    ${SDL3_IMAGE_LIBRARIES}
    ${SDL3_TTF_LIBRARIES}
)

if(WIN32)
    # Windows doesn't need explicit linking to math library
else()
    target_link_libraries(editor m)
endif()

