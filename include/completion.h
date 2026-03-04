#ifndef COMPLETION_H
#define COMPLETION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct CompletionListItem
{
    char* text;
    SDL_Texture* t1;
    struct CompletionListItem* next;
    struct CompletionListItem* prev;

} CompletionListItem;

typedef struct CompletionBoxNode
{
    CompletionListItem* list;
    CompletionListItem* active;
    CompletionListItem* tail;
    int scrollY;
} CompletionBoxNode;

extern int showCompletion;
extern CompletionBoxNode CompletionBox;
void initCompletion(SDL_Renderer *renderer, TTF_Font *font);
int getCompletion(char* word, int n);

#endif