# Contributing to Aether

Thank you for your interest in contributing to Aether! This document provides guidelines for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Contributing Process](#contributing-process)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Documentation](#documentation)
- [Submitting Changes](#submitting-changes)

## Code of Conduct

This project adheres to a code of conduct that we expect all contributors to follow:

- **Be respectful**: Treat all community members with respect and kindness
- **Be inclusive**: Welcome newcomers and help them learn
- **Be constructive**: Provide helpful feedback and suggestions
- **Be patient**: Remember that everyone is learning and growing

## Getting Started

### Prerequisites

Before contributing, ensure you have:

- C compiler (GCC 7+ or Clang 6+)
- SDL2 development libraries
- Git for version control
- Basic knowledge of C programming
- Familiarity with SDL2 (helpful but not required)

### First Time Setup

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/7777Satish/Aether.git
   cd Aether
   ```
3. **Add upstream remote**:
   ```bash
   git remote add upstream https://github.com/7777Satish/Aether.git
   ```
4. **Install dependencies** (see [BUILD.md](BUILD.md))
5. **Build the project**:
   ```bash
   make
   ```
6. **Test the build**:
   ```bash
   ./build/main
   ```

## Development Setup

### Development Environment

Recommended development setup:

- **Editor**: VS Code with C/C++ extension, or any editor with C syntax highlighting
- **Debugger**: GDB for debugging
- **Memory Checker**: Valgrind for memory leak detection
- **Static Analysis**: Cppcheck or similar tools

### Project Structure

Understanding the codebase structure will help you contribute effectively:

```
src/           # Source code files
├── main.c     # Application entry point
├── renderer.c # SDL2 rendering and UI
├── events.c   # Event handling
├── files.c    # File operations
├── parser.c   # Text parsing and syntax highlighting
└── utils.c    # Utility functions

include/       # Header files
assets/        # Images, fonts, and other resources
docs/          # Documentation
```

## Contributing Process

### Types of Contributions

We welcome several types of contributions:

- **Bug fixes**: Fixing issues and crashes
- **Feature additions**: New functionality and improvements
- **Documentation**: Improving or adding documentation
- **Performance**: Optimizations and efficiency improvements
- **Testing**: Adding tests and improving test coverage
- **UI/UX**: Improving user interface and experience

### Finding Issues to Work On

- Check the [GitHub Issues](https://github.com/7777Satish/Aether/issues) page
- Look for issues labeled `good first issue` for beginners
- Issues labeled `help wanted` are especially welcome for contributions
- Feel free to propose new features by opening an issue first

### Before You Start

1. **Check existing issues** to avoid duplicating work
2. **Open an issue** to discuss major changes before implementing
3. **Comment on issues** you want to work on to avoid conflicts
4. **Ask questions** if anything is unclear

## Coding Standards

### C Style Guidelines

Follow these coding standards to maintain consistency:

#### Naming Conventions
```c
// Variables and functions: snake_case
int file_count = 0;
void render_text(char* text);

// Constants: UPPER_SNAKE_CASE
#define MAX_FILES 1000
#define WINDOW_WIDTH 1200

// Structs: PascalCase
typedef struct FileNode {
    char* name;
    struct FileNode* next;
} FileNode;

// Global variables: PascalCase (when necessary)
SDL_Window* MainWindow;
```

#### Code Formatting
```c
// Function definitions
void function_name(int param1, char* param2) {
    // 4-space indentation
    if (condition) {
        // Braces on same line for control structures
        do_something();
    } else {
        do_something_else();
    }
}

// Pointer declarations
char* text;          // Preferred
SDL_Texture* texture;

// Comments
// Single line comments for brief explanations
/**
 * Multi-line comments for function documentation
 * @param param Description of parameter
 * @return Description of return value
 */
```

#### File Organization
```c
// File header with description
/**
 * @file filename.c
 * @brief Brief description of file purpose
 */

// Includes
#include <system_headers.h>
#include "project_headers.h"

// Constants and macros
#define CONSTANT_NAME value

// Type definitions
typedef struct { ... } StructName;

// Global variables (minimize usage)
extern int global_var;

// Function prototypes
void function_prototype(void);

// Function implementations
```

### Memory Management

- **Always check allocation success**:
  ```c
  char* buffer = malloc(size);
  if (!buffer) {
      fprintf(stderr, "Memory allocation failed\n");
      return NULL;
  }
  ```

- **Free all allocated memory**:
  ```c
  free(buffer);
  buffer = NULL;  // Prevent double-free
  ```

- **Check SDL resource creation**:
  ```c
  SDL_Surface* surface = SDL_CreateRGBSurface(...);
  if (!surface) {
      fprintf(stderr, "Surface creation failed: %s\n", SDL_GetError());
      return;
  }
  SDL_FreeSurface(surface);
  ```

### Error Handling

- **Check function return values**
- **Provide meaningful error messages**
- **Use consistent error handling patterns**
- **Log errors for debugging**

Example:
```c
FILE* file = fopen(filename, "r");
if (!file) {
    fprintf(stderr, "Failed to open file '%s': %s\n", filename, strerror(errno));
    return NULL;
}
```

## Testing Guidelines

### Testing Your Changes

Before submitting:

1. **Build without warnings**:
   ```bash
   make clean && make
   ```

2. **Test basic functionality**:
   - Application starts without crashes
   - File explorer works
   - Text editing functions properly

3. **Test edge cases**:
   - Large files
   - Empty files
   - Invalid file paths
   - Memory stress

4. **Memory testing** (if available):
   ```bash
   valgrind --leak-check=full ./build/main
   ```

### Adding Tests

When adding new features:

- Add unit tests for individual functions
- Test error conditions and edge cases
- Ensure tests are deterministic and reproducible

## Documentation

### Code Documentation

- **Document all public functions** with detailed comments
- **Explain complex algorithms** and logic
- **Document global variables** and their purpose
- **Keep comments up-to-date** with code changes

### API Documentation

When modifying public interfaces:
- Update `docs/API.md`
- Include usage examples
- Document parameters and return values
- Note any breaking changes

### User Documentation

For user-facing changes:
- Update README.md if needed
- Add screenshots for UI changes
- Update installation or usage instructions

## Submitting Changes

### Commit Guidelines

Write clear, descriptive commit messages:

```
type(scope): brief description

Longer description if needed, explaining:
- What changed
- Why it changed
- Any side effects or implications

Fixes #123
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style/formatting
- `refactor`: Code refactoring
- `test`: Adding or modifying tests
- `chore`: Maintenance tasks

Examples:
```
feat(parser): add syntax highlighting for Python files

fix(renderer): prevent crash when font loading fails
- Add null checks for font pointers
- Show error message instead of segfaulting
- Gracefully degrade to default font

docs(api): update FileNode structure documentation
```

### Pull Request Process

1. **Create a feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes** following the guidelines above

3. **Test thoroughly** and ensure no regressions

4. **Update documentation** as needed

5. **Commit your changes** with clear messages

6. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

7. **Create a Pull Request** on GitHub with:
   - Clear title and description
   - Reference to related issues
   - Screenshots for UI changes
   - Testing details

### Pull Request Review

- Be patient during the review process
- Address feedback constructively
- Make requested changes promptly
- Ask for clarification if feedback is unclear

### After Your PR is Merged

1. **Delete your feature branch**:
   ```bash
   git branch -d feature/your-feature-name
   git push origin --delete feature/your-feature-name
   ```

2. **Update your local main**:
   ```bash
   git checkout main
   git pull upstream main
   ```

## Getting Help

If you need help:

- **GitHub Issues**: Ask questions by opening an issue
- **Discussions**: Use GitHub Discussions for general questions
- **Email**: Contact maintainers directly for sensitive issues

## Recognition

All contributors will be:
- Listed in the project's contributors
- Credited in release notes for significant contributions
- Thanked publicly for their contributions

Thank you for contributing to Aether! Your efforts help make this project better for everyone.
