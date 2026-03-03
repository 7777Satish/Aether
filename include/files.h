#ifndef FILES_H
#define FILES_H

#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* readFile(char* path);
void open_folder_dialog();

typedef struct FileNode FileNode;


extern FileNode* Folder;
extern char selected_folder[1024];

void clearExplorer();
void initExplorer();
void populateFolder(FileNode *folder);
void writeFile(const char* path, const char* content);

#endif