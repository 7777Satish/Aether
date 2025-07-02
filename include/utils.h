#ifndef UTILS_H
#define UTILS_H

#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FileNode
{
    char *name;
    char *path;
    int isDir;
    int active;
    int hovered;
    int opened;
    int fileType;
    int isDirOpened;
    SDL_Texture *t1;
    SDL_Rect r1;
    SDL_Rect r2;
    struct FileNode *next;
    struct FileNode *child;
    struct FileNode *prev;
} FileNode;

typedef struct FileLine
{
    char *content;
    SDL_Texture *t1;
    struct FileLine *next;
    struct FileLine *prev;
} FileLine;

typedef struct FileBarItem
{
    char *name;
    char *path;
    int active;
    SDL_Texture *t1;
    SDL_Rect r1;
    SDL_Rect r2;
    FileLine *lines;
    struct FileBarItem *next;
    struct FileBarItem *prev;
} FileBarItem;

extern FileBarItem *FileBar;
extern FileBarItem *currentActiveTag;

FileNode *createFileNode(char *name, char *path, int isDir);
FileBarItem *createFileBarNode(char *name, char *path);

void inActiveAllFileNodes();
void addFileBarNode(char *name, char *path);
void handleExplorerItemsHover(FileNode **folder, int x, int y);
void handleExplorerItemsClick(FileNode **folder, int x, int y);

#endif