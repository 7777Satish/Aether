#ifndef FILES_H
#define FILES_H

#include "utils.h"
#include "renderer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

char* readFile(char* path);
void* open_folder_thread();
char* open_folder_dialog();

typedef struct FileNode FileNode;


extern FileNode* Folder;
extern char selected_folder[1024];

void clearExplorer();
void initExplorer();

#endif