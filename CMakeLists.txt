# Basic project info
cmake_minimum_required(VERSION 3.17)
project(Aether)
set(CMAKE_C_STANDARD 11)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(extern/SDL2)
add_subdirectory(extern/SDL2_ttf)
add_subdirectory(extern/SDL2_image)

# Find relavant source files
file(GLOB C_FILES src/*.c)

# Include directories
include_directories(include/)

# Final executable
add_executable(${PROJECT_NAME} ${C_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)