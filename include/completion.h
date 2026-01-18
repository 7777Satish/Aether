#ifndef COMPLETION_H
#define COMPLETION_H

#include <stdio.h>
#include <string.h>
#include "utils.h"

typedef struct CompletionListItem
{
    char* text;
    SDL_Texture* t1;
    struct CompletionListItem* next;
    struct CompletionListItem* prev;

} CompletionListItem;

extern int showCompletion;
extern CompletionListItem* CompletionList;
int getCompletion(char* word, int n);

#endif