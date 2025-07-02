#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FileNode* createFileNode(char* name, char* path, int isDir){
    FileNode* node = (FileNode*)malloc(sizeof(FileNode));
    node->name = strdup(name);
    node->path = strdup(path);
    node->isDir = isDir;
    node->next = NULL;
    node->prev = NULL;
    node->t1 = NULL;
    node->active = 0;
    node->opened = 0;
    node->hovered = 0;
    node->isDirOpened = 0;
    node->child = NULL;
    return node;
}