#ifndef UTILS_H
#define UTILS_H

#include "renderer.h"

typedef struct FileNode {
    char* name;
    char* path;
    int isDir;
    int active;
    int hovered;
    int opened;
    int fileType;
    int isDirOpened;
    SDL_Texture* t1;
    SDL_Rect r1;
    SDL_Rect r2;
    struct FileNode* next;
    struct FileNode* child;
    struct FileNode* prev;
} FileNode;

typedef struct FileBarItem {
    char* name;
    char* path;
    int active;
    SDL_Texture* t1;
    SDL_Rect r1;
    SDL_Rect r1;
    struct FileBarItem* next;
    struct FileBarItem* prev;
} FileBarItem;

FileNode* createFileNode(char* name, char* path, int isDir);
void handleExplorerItemsHover(FileNode** folder, int x, int y);
void handleExplorerItemsClick(FileNode** folder, int x, int y);

#endif
