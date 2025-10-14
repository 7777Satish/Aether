# Architecture Documentation

This document describes the overall architecture, design patterns, and module relationships in CodeEditor_C.

## Table of Contents

- [Overview](#overview)
- [Architectural Principles](#architectural-principles)
- [Module Architecture](#module-architecture)
- [Data Flow](#data-flow)
- [Design Patterns](#design-patterns)
- [Threading Model](#threading-model)
- [Memory Management](#memory-management)
- [Event System](#event-system)
- [Rendering Pipeline](#rendering-pipeline)
- [Extension Points](#extension-points)

## Overview

CodeEditor_C follows a modular, layered architecture designed for maintainability, performance, and extensibility. The system is built around SDL2 for cross-platform graphics and uses a component-based approach for UI elements.

```
┌─────────────────────────────────────────────────────────────┐
│                    User Interface Layer                     │
├─────────────────────────────────────────────────────────────┤
│                    Application Layer                        │
├─────────────────────────────────────────────────────────────┤
│     Renderer    │    Events    │    Files    │   Parser     │
├─────────────────────────────────────────────────────────────┤
│                    Platform Layer (SDL2)                    │
├─────────────────────────────────────────────────────────────┤
│                    Operating System                         │
└─────────────────────────────────────────────────────────────┘
```

## Architectural Principles

### 1. Separation of Concerns
Each module has a single, well-defined responsibility:
- **Renderer**: Graphics and UI rendering
- **Events**: User input handling
- **Files**: File system operations
- **Parser**: Text processing and syntax highlighting
- **Utils**: Common utilities

### 2. Loose Coupling
Modules communicate through well-defined interfaces, minimizing dependencies between components.

### 3. High Cohesion
Related functionality is grouped together within modules.

### 4. Data-Driven Design
UI layouts, color schemes, and configurations are data-driven rather than hard-coded.

### 5. Performance First
Critical paths are optimized for real-time rendering and responsive user interaction.

## Module Architecture

### Core Modules

#### Main Module (`main.c`)
- **Role**: Application entry point and main game loop
- **Responsibilities**:
  - SDL initialization
  - Main event loop
  - High-level coordination between modules
  - Application lifecycle management

#### Renderer Module (`renderer.c/h`)
- **Role**: Graphics rendering and UI management
- **Responsibilities**:
  - SDL2 rendering operations
  - UI component rendering
  - Font management
  - Texture management
  - Layout calculations
  - Theme application

#### Events Module (`events.c/h`)
- **Role**: User input processing
- **Responsibilities**:
  - SDL event polling
  - Mouse and keyboard input handling
  - Event routing to appropriate handlers
  - Input state management

#### Files Module (`files.c/h`)
- **Role**: File system operations
- **Responsibilities**:
  - File I/O operations
  - Directory traversal
  - File explorer functionality
  - File type detection
  - Asynchronous file operations

#### Parser Module (`parser.c/h`)
- **Role**: Text processing and syntax highlighting
- **Responsibilities**:
  - Text parsing
  - Syntax highlighting
  - Language detection
  - Token identification
  - Code analysis

#### Utils Module (`utils.c/h`)
- **Role**: Common utilities and helpers
- **Responsibilities**:
  - String manipulation
  - Memory management helpers
  - Mathematical utilities
  - Platform-specific abstractions

### Module Dependencies

```
main.c
├── renderer.h (Graphics & UI)
├── events.h   (Input handling)
├── files.h    (File operations)
├── parser.h   (Text processing)
└── utils.h    (Utilities)

renderer.h
├── SDL2
├── SDL2_ttf
├── SDL2_image
└── utils.h

files.h
├── renderer.h (for UI elements)
├── utils.h    (for string operations)
├── pthread    (for threading)
└── dirent.h   (for directory operations)

events.h
├── renderer.h (for UI state)
├── files.h    (for file operations)
└── SDL2       (for event handling)

parser.h
├── renderer.h (for syntax highlighting)
└── utils.h    (for string operations)
```

## Data Flow

### 1. Application Startup
```
main() → init() → Load Assets → Initialize UI → Enter Main Loop
```

### 2. Event Processing
```
SDL Event → Event Router → Module Handler → State Update → Render
```

### 3. File Operations
```
User Action → File Thread → File System → Update Explorer → Render
```

### 4. Text Rendering
```
Text Input → Parser → Syntax Highlighter → Renderer → Display
```

## Design Patterns

### 1. Observer Pattern
Used for event handling where multiple components need to respond to user actions.

```c
// Event handlers register for specific events
void registerEventHandler(EventType type, EventHandler handler);
void notifyEventHandlers(Event* event);
```

### 2. Factory Pattern
Used for creating UI components and file nodes.

```c
FileNode* createFileNode(char* name, char* path, int type);
MenuBarNode* createMenuBarNode(char* title, char* icon_path);
```

### 3. State Pattern
UI elements maintain state that affects their behavior and rendering.

```c
typedef enum {
    MENU_STATE_INACTIVE,
    MENU_STATE_ACTIVE,
    MENU_STATE_CLICKED
} MenuState;
```

### 4. Strategy Pattern
Different rendering strategies for different file types and UI themes.

```c
typedef struct {
    void (*render)(void* data);
    void (*update)(void* data);
} RenderStrategy;
```

### 5. Command Pattern
User actions are encapsulated as commands that can be executed, undone, and redone.

```c
typedef struct Command {
    void (*execute)(struct Command* cmd);
    void (*undo)(struct Command* cmd);
    void* data;
} Command;
```

## Threading Model

### Main Thread
- UI rendering and event handling
- SDL operations
- Immediate user feedback

### File Operations Thread
- File system access
- Directory scanning
- File content loading
- Background operations that might block

### Thread Communication
```c
// Thread-safe communication using mutexes
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

void updateFileList() {
    pthread_mutex_lock(&file_mutex);
    // Update shared file data
    pthread_mutex_unlock(&file_mutex);
}
```

## Memory Management

### Allocation Strategy
- **Static Allocation**: Fixed-size arrays for UI elements
- **Dynamic Allocation**: Variable-size data like file content
- **Pool Allocation**: Reusable objects like UI components

### Memory Lifecycle
```c
// Initialization
void* resource = allocateResource();

// Usage
useResource(resource);

// Cleanup
freeResource(resource);
```

### SDL Resource Management
```c
// Texture lifecycle
SDL_Surface* surface = createSurface();
SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
SDL_FreeSurface(surface);  // Free surface immediately
// ... use texture ...
SDL_DestroyTexture(texture);  // Free texture when done
```

## Event System

### Event Flow
```
Hardware Input → SDL Event Queue → Event Dispatcher → Handler Functions
```

### Event Types
- **Mouse Events**: Clicks, movement, wheel
- **Keyboard Events**: Key presses, text input
- **Window Events**: Resize, close, focus
- **Custom Events**: Application-specific events

### Event Handling Pipeline
```c
while (SDL_PollEvent(&event)) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            handleMouseClick(&event);
            break;
        case SDL_KEYDOWN:
            handleKeyPress(&event);
            break;
        // ... other event types
    }
}
```

## Rendering Pipeline

### Frame Rendering Sequence
1. **Clear Screen**: Reset drawing surface
2. **Background**: Draw background elements
3. **UI Components**: Render UI elements in layers
4. **Text**: Render text with syntax highlighting
5. **Overlays**: Draw tooltips, menus, dialogs
6. **Present**: Display the completed frame

### Rendering Layers
```
Layer 5: Tooltips & Dialogs (Highest)
Layer 4: Menus & Dropdowns
Layer 3: Text Content & Editor
Layer 2: UI Components & Panels  
Layer 1: Background & Borders
Layer 0: Window Background (Lowest)
```

### Text Rendering Pipeline
```
Text Input → Tokenization → Syntax Analysis → Font Selection → Render to Surface → Blit to Screen
```

## Extension Points

### 1. Language Support
New programming languages can be added by extending the parser:

```c
typedef struct {
    char* extension;
    SyntaxHighlighter* highlighter;
    FileTypeHandler* handler;
} LanguageDefinition;
```

### 2. Theme System
New themes can be added through configuration:

```c
typedef struct {
    SDL_Color background;
    SDL_Color text;
    SDL_Color keyword;
    SDL_Color comment;
    // ... other colors
} ColorTheme;
```

### 3. Plugin Architecture
Future plugin support through dynamic loading:

```c
typedef struct {
    char* name;
    void (*init)(void);
    void (*render)(void);
    void (*cleanup)(void);
} Plugin;
```

### 4. Custom File Types
Support for new file types through registration:

```c
void registerFileType(char* extension, FileTypeHandler* handler);
```

## Performance Considerations

### 1. Rendering Optimization
- Dirty rectangle updates
- Texture caching
- Batch rendering operations
- Efficient font rendering

### 2. Memory Optimization
- Object pooling for frequently created/destroyed objects
- Efficient string handling
- Minimal memory allocations in critical paths

### 3. File System Optimization
- Asynchronous file operations
- Directory change monitoring
- Lazy loading of file content
- Efficient file tree structures

## Error Handling Strategy

### 1. Graceful Degradation
- Continue operation when non-critical components fail
- Provide fallback functionality

### 2. User Feedback
- Clear error messages for user-facing operations
- Progress indicators for long operations

### 3. Logging
- Comprehensive logging for debugging
- Different log levels for different types of issues

### 4. Recovery
- Automatic recovery from transient failures
- Save user work before critical failures

## Testing Architecture

### 1. Unit Testing
- Individual function testing
- Mock objects for dependencies
- Automated test suites

### 2. Integration Testing
- Module interaction testing
- End-to-end workflow testing

### 3. Performance Testing
- Rendering performance benchmarks
- Memory usage profiling
- Stress testing with large files

This architecture provides a solid foundation for a maintainable, extensible, and performant code editor while keeping the codebase organized and understandable.
