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

FileNode* createFileNode(char* name, char* path, int isDir);

#endif
