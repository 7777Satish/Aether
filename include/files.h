#ifndef FILES_H
#define FILES_H

#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <dirent.h>

char* readFile(const char* path);
int listFiles(const char* folder);
void* open_folder_thread();
char* open_folder_dialog();

extern FileNode* Folder;
extern char selected_folder[1024];

void clearExplorer();
void initExplorer();

#endif