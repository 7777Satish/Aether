# API Documentation

This document provides detailed information about the Aether (CodeEditor) API, including functions, structures, and their usage.

## Table of Contents

- [Core Structures](#core-structures)
- [Renderer Module](#renderer-module)
- [Files Module](#files-module)
- [Events Module](#events-module)
- [Parser Module](#parser-module)
- [Utils Module](#utils-module)
- [Completion Module](#completion-module)

## Core Structures

### FileNode
Represents a file or directory in the explorer. Defined in `utils.h`.

```c
typedef struct FileNode {
    char *name;             // Name of file/directory
    char *path;             // Full path
    int isDir;              // 1 if directory, 0 if file
    int active;             // Selection state
    int hovered;            // Hover state
    int opened;             // Open state
    FileType type;          // File type enum
    int isDirOpened;        // Directory expansion state
    SDL_Texture *t1;        // Name texture
    SDL_FRect r1;           // Main rectangle
    SDL_FRect r2;           // Secondary rectangle
    struct FileNode *next;  // Sibling
    struct FileNode *child; // First child
    struct FileNode *prev;  // Sibling
} FileNode;
```

### FileLine
Represents a line of text in the editor. Defined in `parser.h`.

```c
typedef struct FileLine {
    Token* word;            // Linked list of tokens
    SDL_Texture *t1;        // Cached texture (if used)
    struct FileLine *next;  // Next line
    struct FileLine *prev;  // Previous line
} FileLine;
```

### Token
Represents a syntax token. Defined in `parser.h`.

```c
typedef struct Token {
    char* content;          // Token text
    SDL_Texture* t1;        // Rendered texture
    TokenType type;         // Token type enum
    SDL_Color color;        // Syntax color
    struct Token* next;     // Next token
    struct Token* prev;     // Previous token
    int len;                // Length
} Token;
```

### TOPNAV_MENU_NODE
Represents a top navigation menu item. Defined in `renderer.h`.

```c
typedef struct {
    char name[10];          // Menu name
    char list[10][20];      // Dropdown items
    int isActive;           // Active state
    int clicked;            // Clicked state
    SDL_Texture *texture;   // Rendered texture
    SDL_FRect rect;         // Position and size
} TOPNAV_MENU_NODE;
```

## Renderer Module

This module handles graphics using SDL3.

### Constants & Globals
Defined in `renderer.h` and `renderer.c`.
- `WINDOW_W`, `WINDOW_H`: Default window dimensions.
- `TOPNAV_H`: Height of top navigation bar.
- `MENU_W`: Width of sidebar.

### Functions

*Note: Most rendering logic is internal to `renderer.c` or embedded in `main.c` loop.*

## Files Module

Defined in `files.h`.

### Functions

#### `void initExplorer()`
Initializes the file explorer, scanning the selected directory.

#### `void clearExplorer()`
Frees memory used by the file explorer nodes.

#### `char* readFile(char* path)`
Reads the content of a file into a string.
- **path**: Absolute path to file.
- **Returns**: Malloc'd string containing file content (caller must free).

#### `void writeFile(const char* path, const char* content)`
Writes content to a file.
- **path**: Absolute path.
- **content**: String content to write.

#### `char* open_folder_dialog()`
Opens a native folder selection dialog (using Zenity or other tools).
- **Returns**: Path to selected folder.

#### `void* open_folder_thread()`
Thread function to handle folder opening asynchronously.

## Events Module

Defined in `events.h`. Handles cursor movement and text modification.

### Functions

#### `void handleMouseScroll(int x, int y)`
Handles mouse wheel scrolling.
- **x**: Horizontal scroll amount.
- **y**: Vertical scroll amount.

#### `void moveCursorUp()`
Moves the text cursor up one line.

#### `void moveCursorDown()`
Moves the text cursor down one line.

#### `void moveCursorLeft()`
Moves the text cursor left one character.

#### `void moveCursorRight()`
Moves the text cursor right one character.

#### `void leftDeleteChar()`
Deletes the character before the cursor (Backspace).

#### `void insertChar(char c)`
Inserts a single character at the cursor position.

#### `void insertString(const char* s)`
Inserts a string at the cursor position.

#### `void replaceWord(char* s)`
Replaces the current word/selection with string `s` (used for completion).

#### `void createNewline()`
Inserts a newline character and splits the current line (Enter).

## Parser Module

Defined in `parser.h`.

### Enums

#### `TokenType`
- `TOKEN_UNKNOWN`
- `TOKEN_IDENTIFIER`
- `TOKEN_KEYWORD`
- `TOKEN_TYPE`
- ... (see header for full list)

## Utils Module

Defined in `utils.h`.

### Enums

#### `FileType`
- `AETHER_CLANG`
- `AETHER_CHEADER`
- `AETHER_HTML`
- ...

## Completion Module

Defined in `completion.h`.

### Structures

#### `CompletionListItem`
```c
typedef struct CompletionListItem {
    char* text;
    SDL_Texture* t1;
    struct CompletionListItem* next;
    struct CompletionListItem* prev;
} CompletionListItem;
```

### Functions

#### `int getCompletion(char* word, int n)`
Populates the completion list based on input word.
- **word**: The word prefix to match.
- **n**: Max number of suggestions (?).
- **Returns**: Number of matches found.
