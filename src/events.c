#include "events.h"

void handleMouseScroll(int x, int y)
{

    int dy = 15;
    int dx = 15;

    if (Folder != NULL && MOUSE_X > MENU_BAR_W && MOUSE_X < MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H)
    {
        if (y > 0)
        {
            EXPLORER_SCROLL_Y += dy;
            if (EXPLORER_SCROLL_Y > 0)
                EXPLORER_SCROLL_Y = 0;
        }
        else if (y < 0)
        {
            EXPLORER_SCROLL_Y -= dy;
        }
    }
    else if (FileBar != NULL && MOUSE_X > MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H * 2)
    {

        if (y > 0)
        {
            currentActiveTag->EDITOR_SCROLL_Y += dy * 2;
            if (currentActiveTag->EDITOR_SCROLL_Y > 0)
                currentActiveTag->EDITOR_SCROLL_Y = 0;
        }
        else if (y < 0)
        {
            currentActiveTag->EDITOR_SCROLL_Y -= dy * 2;
        }

        if (x > 0)
        {
            currentActiveTag->EDITOR_SCROLL_X -= dx;
            // if(currentActiveTag->EDITOR_SCROLL_X<-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W) currentActiveTag->EDITOR_SCROLL_X=-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W;
        }
        else if (x < 0)
        {
            currentActiveTag->EDITOR_SCROLL_X += dx;
            if (currentActiveTag->EDITOR_SCROLL_X > 0)
                currentActiveTag->EDITOR_SCROLL_X = 0;
        }
    }
    else if (currentActiveTag != NULL && MOUSE_X > MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H && MOUSE_Y < TOPNAV_H * 2)
    {

        if (x > 0)
        {
            FILEMENU_SCROLL_X -= dx;
            if (FILEMENU_SCROLL_X < -TotalFileBarLength + WINDOW_W - MENU_BAR_W - MENU_W)
                FILEMENU_SCROLL_X = -TotalFileBarLength + WINDOW_W - MENU_BAR_W - MENU_W;
        }
        else if (x < 0)
        {
            FILEMENU_SCROLL_X += dx;
            if (FILEMENU_SCROLL_X > 0)
                FILEMENU_SCROLL_X = 0;
        }
    }
}

void leftDeleteChar(){
    if(!currentActiveTag || !currentActiveTag->currentWord) return;
    if(currentActiveTag->startIndex==0){
        return;
    }
    size_t size = strlen(currentActiveTag->currentWord->content);

    for(int i=currentActiveTag->startIndex-1; i<(int)size; i++){
        currentActiveTag->currentWord->content[i] = currentActiveTag->currentWord->content[i+1];
    }
    currentActiveTag->currentWord->content[size-1] = '\0';
    currentActiveTag->startIndex-=1;
    SDL_Surface* s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
    currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
}

void createNewline(){
    if(!currentActiveTag || !currentActiveTag->currentWord) return;
    
    FileLine *line = (FileLine *)malloc(sizeof(FileLine));
    line->word = currentActiveTag->currentWord;
    if(line->word->prev) line->word->prev->next = NULL;
    else currentActiveTag->currentLine->word = NULL;
    line->word->prev = NULL;
    line->prev = currentActiveTag->currentLine;
    line->next = currentActiveTag->currentLine->next;
    if(currentActiveTag->currentLine->prev) currentActiveTag->currentLine->prev->next = line;
    currentActiveTag->currentLine = line;
}

void insertChar(char c){
    if(!currentActiveTag || !currentActiveTag->currentWord) return;

    if(c==' '){
        Token* space = createToken(" ", 1, (SDL_Color){255, 255, 255, 255});
        Token* next = space;
        space->next = currentActiveTag->currentWord->next;
        currentActiveTag->currentWord->next = space;
        space->prev = currentActiveTag->currentWord;

        if(currentActiveTag->startIndex != currentActiveTag->currentWord->len){
            char* newWord = malloc(currentActiveTag->currentWord->len - currentActiveTag->startIndex);
            for(int i=currentActiveTag->startIndex; i<currentActiveTag->currentWord->len; i++){
                newWord[i - currentActiveTag->startIndex] = currentActiveTag->currentWord->content[i];
            }
            newWord[currentActiveTag->currentWord->len - currentActiveTag->startIndex] = '\0';
            next = createToken(newWord, 0, (SDL_Color){255, 255, 255, 255});
            currentActiveTag->currentWord->content[currentActiveTag->startIndex] = '\0';
            currentActiveTag->currentWord->len = currentActiveTag->startIndex;

            SDL_Surface* s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
            currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_FreeSurface(s1);

            next->next = space->next;
            space->next = next;
            next->prev = space;

        }
        currentActiveTag->currentWord = next;
        currentActiveTag->startIndex = 0;

        return;
    };

    size_t size = strlen(currentActiveTag->currentWord->content);
    char* tempStr = malloc(size+1);
    strcpy(tempStr, currentActiveTag->currentWord->content);

    free(currentActiveTag->currentWord->content);
    currentActiveTag->currentWord->content = malloc(size+2);
    strcpy(currentActiveTag->currentWord->content, tempStr);
    free(tempStr);

    currentActiveTag->currentWord->len = size+1;
    for(int i=size; i>currentActiveTag->startIndex; i--){
        currentActiveTag->currentWord->content[i] = currentActiveTag->currentWord->content[i-1];
    }
    currentActiveTag->currentWord->content[currentActiveTag->startIndex] = c;
    currentActiveTag->currentWord->content[size+1] = '\0';

    currentActiveTag->startIndex += 1;

    SDL_Surface* s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
    currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
    SDL_FreeSurface(s1);
}