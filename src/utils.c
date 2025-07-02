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
    return node;
}