# API Documentation

This document provides detailed information about the Aether API, including functions, structures, and their usage.

## Table of Contents

- [Core Structures](#core-structures)
- [Renderer Module](#renderer-module)
- [Files Module](#files-module)
- [Events Module](#events-module)
- [Parser Module](#parser-module)
- [Utils Module](#utils-module)

## Core Structures

### FileNode
Represents a file or directory in the file explorer.

```c
typedef struct FileNode {
    char* name;              // File/directory name
    char* path;              // Full path
    int type;                // 0 = file, 1 = directory
    int opened;              // Whether file is currently open
    int isDirOpened;         // Whether directory is expanded
    SDL_FRect r1;            // Rectangle for rendering
    struct FileNode* next;   // Next sibling
    struct FileNode* prev;   // Previous sibling
    struct FileNode* child;  // First child (for directories)
} FileNode;
```

### Cursor
Represents the text cursor in the editor.

```c
typedef struct Cursor {
    int w;                   // Width of cursor
    int h;                   // Height of cursor
} Cursor;
```

### MENU_BAR_NODE
Represents a menu item in the left sidebar.

```c
typedef struct MENU_BAR_NODE {
    char* title;             // Menu title
    char* icon_path;         // Path to inactive icon
    char* icon_path_active;  // Path to active icon
    int active;              // Whether menu is active
    int clicked;             // Whether menu is clicked
    SDL_Texture* icon;       // Inactive icon texture
    SDL_Texture* icon_active; // Active icon texture
    SDL_Surface* surface;    // Text surface
    SDL_FRect rect;           // Position and size
    SDL_FRect icon_rect;      // Icon position and size
} MENU_BAR_NODE;
```

### TOPNAV_MENU_NODE
Represents a top navigation menu item.

```c
typedef struct TOPNAV_MENU_NODE {
    char* title;             // Menu title
    char* items[5];          // Menu items array
    int active;              // Whether menu is active
    int clicked;             // Whether menu is clicked
    SDL_Surface* surface;    // Text surface
    SDL_FRect rect;           // Position and size
} TOPNAV_MENU_NODE;
```

## Renderer Module

### Global Variables

```c
extern SDL_Window* window;           // Main window
extern SDL_Renderer* renderer;       // SDL renderer
extern TTF_Font* poppins_regular;    // Regular Poppins font
extern TTF_Font* poppins_bold;       // Bold Poppins font
extern TTF_Font* jetbrains_regular;  // JetBrains Mono font
extern TTF_Font* font2;              // Montserrat font
extern Cursor* cursor;               // Text cursor
```

### Constants

```c
#define WINDOW_W 1200            // Window width
#define WINDOW_H 800             // Window height
#define MENU_W 250               // Left menu width
#define TOPNAV_H 30              // Top navigation height
#define FOOTER_H 25              // Footer height
#define MINIMAP_W 150            // Minimap width
```

### Functions

#### `void init()`
Initializes SDL2, creates window and renderer, loads fonts and assets.

**Usage:**
```c
init(); // Call once at program start
```

**Details:**
- Initializes SDL2 video, TTF, and image subsystems
- Creates main window and renderer
- Loads all required fonts with error checking
- Sets up cursor and logo textures
- Configures menu positions and layouts

#### `void render()`
Main rendering function that draws all UI elements.

**Usage:**
```c
render(); // Call every frame in main loop
```

**Details:**
- Clears the screen
- Renders background, menus, file explorer
- Draws text editor content and cursor
- Handles syntax highlighting
- Updates minimap and status bar

#### `void renderText(char* text, int x, int y, SDL_Color color, TTF_Font* font)`
Renders text at specified position with given color and font.

**Parameters:**
- `text`: String to render
- `x`: X coordinate
- `y`: Y coordinate  
- `color`: Text color (SDL_Color)
- `font`: Font to use

**Usage:**
```c
SDL_Color white = {255, 255, 255, 255};
renderText("Hello World", 100, 100, white, poppins_regular);
```

#### `void renderFileExplorer()`
Renders the file explorer panel with folder tree.

**Usage:**
```c
renderFileExplorer(); // Called automatically by render()
```

## Files Module

### Global Variables

```c
extern char selected_folder[1024];   // Currently selected folder path
extern FileNode* Folder;             // Root folder node
```

### Functions

#### `char* open_folder_dialog()`
Opens a system folder selection dialog using Zenity.

**Returns:** Path to selected folder or NULL if cancelled

**Usage:**
```c
char* folder_path = open_folder_dialog();
if (folder_path) {
    printf("Selected: %s\n", folder_path);
}
```

#### `void* open_folder_thread(void* arg)`
Thread function for opening folder dialog without blocking UI.

**Parameters:**
- `arg`: Thread argument (unused)

**Returns:** NULL

**Usage:**
```c
pthread_t folder_thread;
pthread_create(&folder_thread, NULL, open_folder_thread, NULL);
pthread_detach(folder_thread);
```

#### `void initExplorer()`
Initializes the file explorer by reading the selected folder.

**Usage:**
```c
initExplorer(); // Called after folder selection
```

**Details:**
- Scans selected folder for files and directories
- Creates FileNode structure for each item
- Sets up rendering rectangles
- Handles file type detection

#### `void clearExplorer()`
Clears the current file explorer and frees memory.

**Usage:**
```c
clearExplorer(); // Call before loading new folder
```

#### `char* readFile(char* path)`
Reads entire file content into memory.

**Parameters:**
- `path`: File path to read

**Returns:** File content as string or NULL on error

**Usage:**
```c
char* content = readFile("/path/to/file.txt");
if (content) {
    // Use content
    free(content); // Remember to free
}
```

#### `FileNode* createFileNode(char* name, char* path, int type)`
Creates a new FileNode structure.

**Parameters:**
- `name`: File/directory name
- `path`: Full path
- `type`: 0 for file, 1 for directory

**Returns:** Pointer to new FileNode

**Usage:**
```c
FileNode* node = createFileNode("example.c", "/full/path/example.c", 0);
```

## Events Module

### Functions

#### `void handleEvents(SDL_Event* event)`
Main event handler for all user input.

**Parameters:**
- `event`: SDL event structure

**Usage:**
```c
SDL_Event event;
while (SDL_PollEvent(&event)) {
    handleEvents(&event);
}
```

#### `void handleMouseClick(int x, int y)`
Handles mouse click events at specified coordinates.

**Parameters:**
- `x`: Mouse X coordinate
- `y`: Mouse Y coordinate

**Usage:**
```c
handleMouseClick(event.button.x, event.button.y);
```

#### `void handleKeyPress(SDL_Keycode key)`
Handles keyboard input events.

**Parameters:**
- `key`: SDL keycode

**Usage:**
```c
handleKeyPress(event.key.keysym.sym);
```

#### `void handleExplorerItemsClick(FileNode** folder, int x, int y)`
Handles clicks on file explorer items.

**Parameters:**
- `folder`: Pointer to folder node
- `x`: Mouse X coordinate
- `y`: Mouse Y coordinate

**Usage:**
```c
handleExplorerItemsClick(&Folder, mouse_x, mouse_y);
```

## Parser Module

### Functions

#### `void parseFile(char* content, char* filename)`
Parses file content for syntax highlighting.

**Parameters:**
- `content`: File content string
- `filename`: Name of file (for type detection)

**Usage:**
```c
char* content = readFile("example.c");
parseFile(content, "example.c");
```

#### `int getFileType(char* filename)`
Determines file type based on extension.

**Parameters:**
- `filename`: Name of file

**Returns:** File type constant

**Usage:**
```c
int type = getFileType("example.c"); // Returns C_FILE type
```

#### `void highlightSyntax(char* line, int line_number)`
Applies syntax highlighting to a line of text.

**Parameters:**
- `line`: Line of text to highlight
- `line_number`: Line number in file

**Usage:**
```c
highlightSyntax("int main() {", 1);
```

## Utils Module

### Functions

#### `char* getFileExtension(char* filename)`
Extracts file extension from filename.

**Parameters:**
- `filename`: Name of file

**Returns:** Pointer to extension string

**Usage:**
```c
char* ext = getFileExtension("example.c"); // Returns "c"
```

#### `void trimWhitespace(char* str)`
Removes leading and trailing whitespace from string.

**Parameters:**
- `str`: String to trim (modified in place)

**Usage:**
```c
char text[] = "  hello world  ";
trimWhitespace(text); // text becomes "hello world"
```

#### `int isWhitespace(char c)`
Checks if character is whitespace.

**Parameters:**
- `c`: Character to check

**Returns:** 1 if whitespace, 0 otherwise

**Usage:**
```c
if (isWhitespace(' ')) {
    // Handle whitespace
}
```

#### `void copyString(char* dest, const char* src, int max_len)`
Safely copies string with length limit.

**Parameters:**
- `dest`: Destination buffer
- `src`: Source string
- `max_len`: Maximum characters to copy

**Usage:**
```c
char buffer[100];
copyString(buffer, "Hello World", sizeof(buffer));
```

## Error Handling

All functions include proper error handling:

- Font loading functions exit with error messages if fonts cannot be loaded
- File operations return NULL on failure with error logging
- Surface creation is checked for NULL before use
- Memory allocation is verified before proceeding

## Thread Safety

- File operations use pthread for non-blocking UI
- Shared variables should be protected with mutexes in multi-threaded contexts
- SDL operations should be performed on the main thread

## Memory Management

- All dynamically allocated memory must be freed
- SDL surfaces and textures are properly cleaned up
- FileNode structures use linked lists with proper cleanup functions
