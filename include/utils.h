#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

typedef struct {
    char* name;
    char* path;
    int isDir;
    int active;
    int fileType;
    SDL_Texture* t1;
    SDL_Rect r1;
    SDL_Rect r2;
    struct FileNode* next;
    struct FileNode* prev;
} FileNode;

FileNode* createFileNode(char* name, char* path, int isDir);

#endif
