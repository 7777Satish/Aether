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

void leftDeleteChar()
{
    if (!currentActiveTag && !currentActiveTag->currentWord)
        return;

    if (currentActiveTag->startIndex == 0)
    {
        size_t size1 = strlen(currentActiveTag->currentWord->content);
        currentActiveTag->currentWord->len = size1;
        if (currentActiveTag->currentWord->prev)
        {
            size_t size2 = strlen(currentActiveTag->currentWord->prev->content);
            currentActiveTag->currentWord->prev->len = size2;

            int len = size1 + size2;

            currentActiveTag->startIndex = size2;

            currentActiveTag->currentWord->prev->content = realloc(currentActiveTag->currentWord->prev->content, len + 1);
            for (int i = 0; i < (int)size1; i++)
            {
                currentActiveTag->currentWord->prev->content[size2 + i] = currentActiveTag->currentWord->content[i];
            }

            currentActiveTag->currentWord->prev->content[len] = '\0';
            currentActiveTag->currentWord->prev->len = len;

            // printf("%s %d\n", currentActiveTag->currentWord->prev->content, currentActiveTag->currentWord->prev->len);
            Token *temp = currentActiveTag->currentWord;
            temp->prev->next = temp->next;
            if (temp->next)
                temp->next->prev = temp->prev;

            SDL_DestroyTexture(temp->t1);
            free(temp->content);

            currentActiveTag->currentWord = temp->prev;
            free(temp);
        }
        else
        {
            return;
        }
    }
    size_t size = strlen(currentActiveTag->currentWord->content);
    printf("%d\n", currentActiveTag->startIndex);
    for (int i = currentActiveTag->startIndex - 1; i < (int)size; i++)
    {
        currentActiveTag->currentWord->content[i] = currentActiveTag->currentWord->content[i + 1];
    }
    currentActiveTag->currentWord->content[size - 1] = '\0';
    if(currentActiveTag->startIndex>0){
        currentActiveTag->currentWord->len--;
        currentActiveTag->startIndex -= 1;
    }
    SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
    SDL_DestroyTexture(currentActiveTag->currentWord->t1);
    currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
}

void createNewline()
{
    if (!currentActiveTag || !currentActiveTag->currentWord)
        return;
    if (currentActiveTag->startIndex != 0 && currentActiveTag->startIndex != currentActiveTag->currentWord->len)
    {
        char *newWord = malloc(currentActiveTag->currentWord->len - currentActiveTag->startIndex);
        Token *next = NULL;

        for (int i = currentActiveTag->startIndex; i < currentActiveTag->currentWord->len; i++)
        {
            newWord[i - currentActiveTag->startIndex] = currentActiveTag->currentWord->content[i];
        }
        newWord[currentActiveTag->currentWord->len - currentActiveTag->startIndex] = '\0';
        next = createToken(newWord, 0, (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->content[currentActiveTag->startIndex] = '\0';
        currentActiveTag->currentWord->len = currentActiveTag->startIndex;

        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_FreeSurface(s1);

        next->next = currentActiveTag->currentWord->next;
        currentActiveTag->currentWord->next = next;
        next->prev = currentActiveTag->currentWord;

        currentActiveTag->currentWord = next;
        currentActiveTag->startIndex = 0;
    }

    FileLine *currentLine = currentActiveTag->currentLine;
    Token *currentWord = currentActiveTag->currentWord;

    FileLine *line = (FileLine *)malloc(sizeof(FileLine));
    line->next = currentLine->next;
    line->prev = currentLine;
    if (currentLine->next)
        currentLine->next->prev = line;
    currentLine->next = line;

    currentActiveTag->currentLine = line;

    line->word = currentWord;
    if (currentWord->prev)
        currentWord->prev->next = NULL;
    else
        currentLine->word = NULL;
    currentWord->prev = NULL;
}

void insertChar(char c)
{
    if (!currentActiveTag || !currentActiveTag->currentWord)
        return;
    /*
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
    */

    size_t size = strlen(currentActiveTag->currentWord->content);

    currentActiveTag->currentWord->content = realloc(currentActiveTag->currentWord->content, size + 2);

    currentActiveTag->currentWord->len = size + 1;
    for (int i = size; i > currentActiveTag->startIndex; i--)
    {
        currentActiveTag->currentWord->content[i] = currentActiveTag->currentWord->content[i - 1];
    }
    currentActiveTag->currentWord->content[currentActiveTag->startIndex] = c;
    currentActiveTag->currentWord->content[size + 1] = '\0';

    currentActiveTag->startIndex += 1;

    // Convert edited text to tokens
    FileLine *line1 = parseText(currentActiveTag->currentWord->content);

    if (line1)
    {
        Token *tempWord = line1->word;
        Token *old = currentActiveTag->currentWord;

        if (old->prev)
            old->prev->next = tempWord;
        else
            currentActiveTag->currentLine->word = tempWord;

        tempWord->prev = old->prev;

        int startInd = 0;
        int len = 0;

        Token *p = NULL;
        Token *lastToken = tempWord;

        while (tempWord)
        {
            if (!p)
            {
                len += tempWord->len;
                if (currentActiveTag->startIndex < len)
                {
                    p = tempWord;
                    startInd = currentActiveTag->startIndex - (len - tempWord->len);
                }
            }

            if (!tempWord->next)
                lastToken = tempWord;

            tempWord = tempWord->next;
        }

        if (!p)
        {
            p = lastToken;
            startInd = lastToken->len;
        }

        lastToken->next = old->next;
        if (lastToken->next)
            lastToken->next->prev = lastToken;

        currentActiveTag->currentWord = p;
        currentActiveTag->startIndex = startInd;
        
        printf("%d\n", currentActiveTag->startIndex);

        // free(old->content);
        // SDL_DestroyTexture(old->t1);
        // free(old);
    }
    // Render as plain text
    else
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_FreeSurface(s1);
    }
}