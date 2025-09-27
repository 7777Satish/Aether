#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "renderer.h"
#include "parser.h"

typedef enum {
    CLANG,
    CHEADER,
    HTML,
    CSS,
    TXT,
    IMG,
} FileType;

typedef struct FileNode
{
    char *name;
    char *path;
    int isDir;
    int active;
    int hovered;
    int opened;
    FileType type;
    int isDirOpened;
    SDL_Texture *t1;
    SDL_Rect r1;
    SDL_Rect r2;
    struct FileNode *next;
    struct FileNode *child;
    struct FileNode *prev;
} FileNode;

typedef struct FileLine FileLine;

typedef struct Token Token;

typedef struct FileBarItem{
    char *name;
    char *path;
    int active;
    SDL_Texture *t1;
    SDL_Rect r1;
    SDL_Rect r2;
    FileLine *lines;

    int EDITOR_SCROLL_X;
    int EDITOR_SCROLL_Y;
    Token* currentWord;
    FileLine* currentLine;
    int cursorX;
    int cursorY;

    int startIndex;
    struct FileBarItem *next;
    struct FileBarItem *prev;
} FileBarItem;

extern FileBarItem *FileBar;
extern FileBarItem *currentActiveTag;
extern int TotalFileBarLength;

FileNode *createFileNode(char *name, char *path, int isDir);
FileBarItem *createFileBarNode(char *name, char *path);

void inActiveAllFileNodes();
void addFileBarNode(char *name, char *path);
void handleExplorerItemsHover(FileNode **folder, int x, int y);
void handleExplorerItemsClick(FileNode **folder, int x, int y);

#endif