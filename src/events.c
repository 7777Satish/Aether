#include "events.h"

void handleMouseScroll(int x, int y)
{

    int dy = 15;
    // int dy = EDITOR_FONT_HEIGHT;
    int dx = 15;

    if (Folder != NULL && MOUSE_X > 0 && MOUSE_X < MENU_W && MOUSE_Y > TOPNAV_H)
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
    else if (FileBar != NULL && MOUSE_X > MENU_W && MOUSE_Y > TOPNAV_H * 2)
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
    else if (currentActiveTag != NULL && MOUSE_X > MENU_W && MOUSE_Y > TOPNAV_H && MOUSE_Y < TOPNAV_H * 2)
    {

        if (x > 0)
        {
            FILEMENU_SCROLL_X -= dx;
            if (FILEMENU_SCROLL_X < -TotalFileBarLength + WINDOW_W - MENU_W)
                FILEMENU_SCROLL_X = -TotalFileBarLength + WINDOW_W - MENU_W;
        }
        else if (x < 0)
        {
            FILEMENU_SCROLL_X += dx;
            if (FILEMENU_SCROLL_X > 0)
                FILEMENU_SCROLL_X = 0;
        }
    }
}

void moveCursorUp()
{
    if (!currentActiveTag || !currentActiveTag->currentLine || !currentActiveTag->currentWord)
        return;

    if (!currentActiveTag->currentLine->prev)
    {
        currentActiveTag->currentWord = currentActiveTag->currentLine->word;
        return;
    }

    int currentX = 0;
    Token *currentNode = currentActiveTag->currentLine->word;
    while (currentNode != currentActiveTag->currentWord)
    {
        currentX += currentNode->len;
        currentNode = currentNode->next;
    }
    currentX += currentActiveTag->startIndex;

    currentActiveTag->currentLine = currentActiveTag->currentLine->prev;
    currentActiveTag->currentWord = currentActiveTag->currentLine->word;

    int newX = 0;
    currentNode = currentActiveTag->currentWord;
    while (newX + currentNode->len < currentX && currentNode->next)
    {
        newX += currentNode->len;
        currentNode = currentNode->next;
    }
    currentActiveTag->currentWord = currentNode;
    currentActiveTag->startIndex = currentX - newX;

    if (currentActiveTag->startIndex > currentActiveTag->currentWord->len)
    {
        currentActiveTag->startIndex = currentActiveTag->currentWord->len;
    }
}

void moveCursorDown()
{
    if (!currentActiveTag || !currentActiveTag->currentLine || !currentActiveTag->currentWord)
        return;

    if (!currentActiveTag->currentLine->next)
    {
        Token *node = currentActiveTag->currentWord;
        while (node->next)
        {
            node = node->next;
        }
        currentActiveTag->currentWord = node;
        currentActiveTag->startIndex = node->len;
        return;
    }

    int currentX = 0;
    Token *currentNode = currentActiveTag->currentLine->word;
    while (currentNode != currentActiveTag->currentWord)
    {
        currentX += currentNode->len;
        currentNode = currentNode->next;
    }
    currentX += currentActiveTag->startIndex;

    currentActiveTag->currentLine = currentActiveTag->currentLine->next;
    currentActiveTag->currentWord = currentActiveTag->currentLine->word;

    int newX = 0;
    currentNode = currentActiveTag->currentWord;
    while (newX + currentNode->len < currentX && currentNode->next)
    {
        newX += currentNode->len;
        currentNode = currentNode->next;
    }
    currentActiveTag->currentWord = currentNode;
    currentActiveTag->startIndex = currentX - newX;

    if (currentActiveTag->startIndex > currentActiveTag->currentWord->len)
    {
        currentActiveTag->startIndex = currentActiveTag->currentWord->len;
    }
}

void moveCursorLeft()
{
    if (!currentActiveTag || !currentActiveTag->currentLine)
        return;

    if (currentActiveTag->startIndex <= 0)
    {
        if (currentActiveTag->currentWord->prev)
        {
            currentActiveTag->currentWord = currentActiveTag->currentWord->prev;
            currentActiveTag->startIndex = currentActiveTag->currentWord->len - 1;
        }
        else if (currentActiveTag->currentLine->prev)
        {
            Token *word = currentActiveTag->currentLine->prev->word;
            while (word->next)
            {
                word = word->next;
            }
            currentActiveTag->currentWord = word;
            currentActiveTag->currentLine = currentActiveTag->currentLine->prev;
            currentActiveTag->startIndex = word->len;
        }
        return;
    }

    currentActiveTag->startIndex--;
}

void moveCursorRight()
{
    if (!currentActiveTag || !currentActiveTag->currentLine)
        return;

    if (currentActiveTag->startIndex == currentActiveTag->currentWord->len || strcmp(currentActiveTag->currentWord->content, "") == 0)
    {
        if (currentActiveTag->currentWord->next)
        {
            currentActiveTag->currentWord = currentActiveTag->currentWord->next;
            currentActiveTag->startIndex = 1;
        }
        else if (currentActiveTag->currentLine->next)
        {
            currentActiveTag->currentWord = currentActiveTag->currentLine->next->word;
            currentActiveTag->currentLine = currentActiveTag->currentLine->next;

            currentActiveTag->startIndex = 0;
        }
        return;
    }

    currentActiveTag->startIndex++;
}

void leftDeleteChar()
{
    if (!currentActiveTag || !currentActiveTag->currentWord)
        return;

    // if (!currentActiveTag->currentWord->len)
    // {

    //     if (!currentActiveTag->currentWord->prev)
    //     {
    //         SDL_DestroyTexture(currentActiveTag->currentWord->t1);
    //         if (!currentActiveTag->currentLine->prev)
    //             return;

    //         Token *temp = currentActiveTag->currentLine->prev->word;
    //         FileLine *tempLine = currentActiveTag->currentLine;

    //         if (temp)
    //         {
    //             while (temp->next)
    //             {
    //                 temp = temp->next;
    //             }

    //             temp->next = tempLine->word;
    //             currentActiveTag->currentWord->prev = temp;
    //         }
    //         else
    //         {
    //             tempLine->prev->word = currentActiveTag->currentLine->word;
    //         }

    //         tempLine->prev->next = tempLine->next;
    //         if (tempLine->next)
    //             tempLine->next->prev = tempLine->prev;

    //         currentActiveTag->currentLine = tempLine->prev;
            
    //         free(tempLine);
    //         return;
    //     }

    // }

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

            char *newContent = realloc(currentActiveTag->currentWord->prev->content, len + 1);
            if (!newContent)
                return; // out of memory
            currentActiveTag->currentWord->prev->content = newContent;

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
            if (!currentActiveTag->currentLine->prev)
                return;

            Token *temp = currentActiveTag->currentLine->prev->word;
            FileLine *tempLine = currentActiveTag->currentLine;

            if (temp)
            {
                while (temp->next)
                {
                    temp = temp->next;
                }

                temp->next = tempLine->word;
                currentActiveTag->currentWord->prev = temp;
            }
            else
            {
                tempLine->prev->word = currentActiveTag->currentLine->word;
            }

            tempLine->prev->next = tempLine->next;
            if (tempLine->next)
                tempLine->next->prev = tempLine->prev;

            currentActiveTag->currentLine = tempLine->prev;

            free(tempLine);

            return;
        }
    }

    size_t size = strlen(currentActiveTag->currentWord->content);
    // printf("%d\n", currentActiveTag->startIndex);
    if (currentActiveTag->startIndex > 0)
    {
        for (int i = currentActiveTag->startIndex - 1; i < (int)size; i++)
        {
            currentActiveTag->currentWord->content[i] = currentActiveTag->currentWord->content[i + 1];
        }

        currentActiveTag->currentWord->content[size - 1] = '\0';

        currentActiveTag->currentWord->len--;
        currentActiveTag->startIndex -= 1;
    }
    SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
    SDL_DestroyTexture(currentActiveTag->currentWord->t1);
    currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);

    SDL_FreeSurface(s1);
}
void createNewline()
{
    if (!currentActiveTag || !currentActiveTag->currentWord)
        return;

    // Split the word into 2 if cursor is in middle
    if (currentActiveTag->startIndex > 0 && currentActiveTag->startIndex < currentActiveTag->currentWord->len)
    {
        int remainLen = currentActiveTag->currentWord->len - currentActiveTag->startIndex;
        char *newText = malloc(remainLen + 1);
        if (!newText)
            return;

        memcpy(newText, currentActiveTag->currentWord->content + currentActiveTag->startIndex, remainLen);
        newText[remainLen] = '\0';

        Token *newToken = createToken(newText, 0, (SDL_Color){255, 255, 255, 255});
        free(newText); // createToken should own its own content

        // Cut original token
        currentActiveTag->currentWord->content[currentActiveTag->startIndex] = '\0';
        currentActiveTag->currentWord->len = currentActiveTag->startIndex;

        // Re-render old token
        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_FreeSurface(s1);

        // Insert new token after current
        newToken->next = currentActiveTag->currentWord->next;
        if (newToken->next)
            newToken->next->prev = newToken;
        currentActiveTag->currentWord->next = newToken;
        newToken->prev = currentActiveTag->currentWord;

        currentActiveTag->currentWord = newToken;
        currentActiveTag->startIndex = 0;
    }

    else if (currentActiveTag->startIndex == currentActiveTag->currentWord->len)
    {
        if (currentActiveTag->currentWord->next)
        {
            currentActiveTag->currentWord = currentActiveTag->currentWord->next;
            currentActiveTag->startIndex = 0;
        }
        else
        {
            Token *newToken = createToken("", 1, (SDL_Color){255, 255, 255, 255});

            // Insert new token after current
            newToken->next = currentActiveTag->currentWord->next;
            if (newToken->next)
                newToken->next->prev = newToken;
            currentActiveTag->currentWord->next = newToken;
            newToken->prev = currentActiveTag->currentWord;

            currentActiveTag->currentWord = newToken;
            currentActiveTag->startIndex = 0;
        }
    }

    // ---- Create newline and move token chain ----
    FileLine *currentLine = currentActiveTag->currentLine;
    FileLine *newLine = malloc(sizeof(FileLine));
    memset(newLine, 0, sizeof(FileLine)); // initialize

    newLine->next = currentLine->next;
    newLine->prev = currentLine;
    if (currentLine->next)
        currentLine->next->prev = newLine;
    currentLine->next = newLine;

    // Move words to new line
    newLine->word = currentActiveTag->currentWord;
    if (currentActiveTag->currentWord->prev)
        currentActiveTag->currentWord->prev->next = NULL;
    else
        currentLine->word = NULL;

    currentActiveTag->currentWord->prev = NULL;
    currentActiveTag->currentLine = newLine;
}

void insertChar(char c)
{
    if (!currentActiveTag || !currentActiveTag->currentLine)
        return;
    IS_SELECTING = 0;
    if (!currentActiveTag->currentWord)
    {
        char str[1];
        str[0] = c;
        FileLine *line1 = parseText(str);
        currentActiveTag->currentLine->word = line1->word;
        if (line1->word)
            currentActiveTag->startIndex = 1;
        return;
    }

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

        // printf("%d\n", currentActiveTag->startIndex);

        free(old->content);
        SDL_DestroyTexture(old->t1);
        free(old);
    }
    // Render as plain text
    else
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_FreeSurface(s1);
    }
}