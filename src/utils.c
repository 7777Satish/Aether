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


void handleExplorerItemsHover(FileNode** folder, int x, int y){
    FileNode* node = (*folder)->child;
    int i = 0;
    while (node!=NULL)
    {

        if(x>MENU_BAR_W + MENU_PAD_X && x<MENU_BAR_W + MENU_W - MENU_PAD_X && y>node->r1.y-MENU_PAD_Y/4 && y<node->r1.y+node->r1.h+MENU_PAD_Y/4){
            node->hovered = 1;
        } else {
            node->hovered = 0;
        }

        if(node->isDir && node->child){
            handleExplorerItemsHover(&node, x, y);
        }

        node = node->next;
        i=+1;
    }
}


void handleExplorerItemsClick(FileNode** folder, int x, int y){
    FileNode* node = (*folder)->child;
    int i=0;
    while (node!=NULL)
    {

        if(x>MENU_BAR_W + MENU_PAD_X && x<MENU_BAR_W + MENU_W - MENU_PAD_X && y>node->r1.y-MENU_PAD_Y/4 && y<node->r1.y+node->r1.h+MENU_PAD_Y/4){
            if(node->isDir){
                node->opened = !node->opened;
                node->active = 1;
            }
        } else {
            node->active = 0;
        }

        if(node->isDir && node->child){
            handleExplorerItemsClick(&node, x, y);
        }

        node = node->next;
        i+=1;
    }
}