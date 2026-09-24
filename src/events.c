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
            if (currentActiveTag->visibleLine && currentActiveTag->visibleLine->next)
            {
                currentActiveTag->EDITOR_SCROLL_Y -= dy * 2;
            }
        }

        if (x > 0)
        {
            currentActiveTag->EDITOR_SCROLL_X += dx;

            if (currentActiveTag->EDITOR_SCROLL_X > 0)
                currentActiveTag->EDITOR_SCROLL_X = 0;
            // if(currentActiveTag->EDITOR_SCROLL_X<-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W) currentActiveTag->EDITOR_SCROLL_X=-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W;
        }
        else if (x < 0)
        {
            currentActiveTag->EDITOR_SCROLL_X -= dx;
        }
    }
    else if (currentActiveTag != NULL && MOUSE_X > MENU_W && MOUSE_Y > TOPNAV_H && MOUSE_Y < TOPNAV_H * 2)
    {

        if (x > 0)
        {
            FILEMENU_SCROLL_X += dx;
            if (FILEMENU_SCROLL_X > 0)
                FILEMENU_SCROLL_X = 0;
        }
        else if (x < 0)
        {
            FILEMENU_SCROLL_X -= dx;
            if (FILEMENU_SCROLL_X < -TotalFileBarLength + WINDOW_W - MENU_W)
                FILEMENU_SCROLL_X = -TotalFileBarLength + WINDOW_W - MENU_W;
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

    FileLine *newLine = currentActiveTag->currentLine->prev;
    if (newLine->collapsed == 2)
    {
        int n = 1;
        int exit = 0;
        while (!exit && newLine->prev)
        {
            newLine = newLine->prev;
            if (newLine->collapsed == 2)
            {
                n += 1;
            }
            if (newLine->collapsed == 1)
            {
                n -= 1;
            }
            if (n == 0)
            {
                exit = 1;
                break;
            }
        }
    }

    currentActiveTag->currentLine = newLine;
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

    FileLine *newLine = currentActiveTag->currentLine->next;
    if (newLine->prev->collapsed == 1)
    {
        int n = 1;
        int exit = 0;
        while (n != 0 && newLine)
        {
            if (newLine->collapsed == 1)
            {
                n += 1;
            }
            if (newLine->collapsed == 2)
            {
                n -= 1;
            }
            newLine = newLine->next;
        }
    }

    currentActiveTag->currentLine = newLine;
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
            FileLine *prevLine = currentActiveTag->currentLine->prev;
            if (prevLine->collapsed == 2)
            {
                int n = 1;
                int exit = 0;
                while (n != 0 && prevLine->prev)
                {
                    prevLine = prevLine->prev;
                    if (prevLine->collapsed == 2)
                    {
                        n += 1;
                    }
                    if (prevLine->collapsed == 1)
                    {
                        n -= 1;
                    }
                }
            }
            Token *word = prevLine->word;
            while (word->next)
            {
                word = word->next;
            }
            currentActiveTag->currentWord = word;
            currentActiveTag->currentLine = prevLine;
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

            FileLine *newLine = currentActiveTag->currentLine->next;
            if (newLine->prev->collapsed == 1)
            {
                int n = 1;
                while (n != 0 && newLine->next)
                {
                    if (newLine->collapsed == 1)
                    {
                        n += 1;
                    }
                    if (newLine->collapsed == 2)
                    {
                        n -= 1;
                    }
                    newLine = newLine->next;
                }
            }

            currentActiveTag->currentLine = newLine;
            currentActiveTag->currentWord = newLine->word;

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
    /*
        if (!currentActiveTag->currentWord->len)
        {

            if (!currentActiveTag->currentWord->prev)
            {
                SDL_DestroyTexture(currentActiveTag->currentWord->t1);
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
    */

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
                if (currentActiveTag->currentLine->prev->word->len)
                {
                    if (tempLine->word->len)
                    {
                        temp->next = tempLine->word;
                        currentActiveTag->currentWord->prev = temp;
                    }
                    else
                    {
                        temp->next = NULL;
                        SDL_DestroyTexture(tempLine->word->t1);
                        free(tempLine->word);
                        currentActiveTag->currentWord = temp;
                        currentActiveTag->startIndex = temp->len;
                    }
                }
                else
                {
                    tempLine->prev->word = currentActiveTag->currentLine->word;
                }
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
    SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, strlen(currentActiveTag->currentWord->content), (SDL_Color){255, 255, 255, 255});
    SDL_DestroyTexture(currentActiveTag->currentWord->t1);
    currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
    // SDL_SetTextureScaleMode(currentActiveTag->currentWord->t1, SDL_SCALEMODE_NEAREST);

    SDL_DestroySurface(s1);
    currentActiveTag->minimapDirty = 1;
}

void createNewline()
{
    if (!currentActiveTag || !currentActiveTag->currentWord)
        return;
    int tabSize = 4;
    int spaces = 0;
    Token *t = currentActiveTag->currentLine->word;
    while (t && strcmp(t->content, " ") == 0)
    {
        spaces += 1;
        t = t->next;
    }
    // spaces = (spaces/tabSize)*tabSize;

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
        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, strlen(currentActiveTag->currentWord->content), (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(currentActiveTag->currentWord->t1, SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(s1);

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

    Token *tabsHead = NULL;
    Token *tabsTail = NULL;
    for (int i = 0; i < spaces; i++)
    {
        Token *t = createToken(" ", 1, (SDL_Color){255, 255, 255, 255});
        if (tabsHead)
        {
            t->next = tabsHead;
            tabsHead->prev = t;
        }
        if (!tabsTail)
            tabsTail = t;
        tabsHead = t;
    }

    currentActiveTag->currentWord->prev = tabsTail;
    currentActiveTag->currentLine = newLine;
    if (tabsTail)
        tabsTail->next = currentActiveTag->currentWord;
    if (tabsHead)
        currentActiveTag->currentLine->word = tabsHead;
    currentActiveTag->minimapDirty = 1;
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
        if (!currentActiveTag->currentLine)
            currentActiveTag->currentLine = line1;
        currentActiveTag->currentLine->word = line1->word;
        if (line1->word)
            currentActiveTag->startIndex = 0;
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
        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, strlen(currentActiveTag->currentWord->content), (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(currentActiveTag->currentWord->t1, SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(s1);
        currentActiveTag->minimapDirty = 1;
    }
}

void insertString(const char *str)
{
    if (!currentActiveTag || !currentActiveTag->currentLine)
        return;

    if (!str)
    {
        printf("No string given\n");
        return;
    }

    if (strlen(str) == 0)
        return;
    char *s = malloc(strlen(str) + 1);
    strcpy(s, str);
    IS_SELECTING = 0;

    if (!currentActiveTag->currentWord)
    {
        FileLine *line1 = parseText(s);
        currentActiveTag->currentLine->word = line1 ? line1->word : NULL;

        if (line1->word)
        {
            int l = 1;
            currentActiveTag->startIndex = l;
        }
        return;
    }

    size_t size = strlen(currentActiveTag->currentWord->content);
    size_t size2 = strlen(s);
    int offRight = size - currentActiveTag->startIndex;

    // currentActiveTag->currentWord->content = realloc(currentActiveTag->currentWord->content, size + size2 + 1);

    currentActiveTag->currentWord->len = size + size2;

    char *tempStr = malloc(size + size2 + 1);
    tempStr[0] = '\0';
    strncat(tempStr, currentActiveTag->currentWord->content, currentActiveTag->startIndex);
    strcat(tempStr, s);
    strcat(tempStr, &currentActiveTag->currentWord->content[currentActiveTag->startIndex]);
    free(currentActiveTag->currentWord->content);
    currentActiveTag->currentWord->content = tempStr;

    currentActiveTag->startIndex += size2;

    // Convert edited text to tokens
    FileLine *line1 = parseText(currentActiveTag->currentWord->content);

    if (line1)
    {
        Token *old = currentActiveTag->currentWord;
        if (line1->word)
        {
            if (old->prev)
                old->prev->next = line1->word;
            else
                currentActiveTag->currentLine->word = line1->word;

            line1->word->prev = old->prev;
        }

        int len = 0;

        FileLine *line = line1;
        if (line->next)
        {
            while (line->next)
            {
                line = line->next;
            }

            line->next = currentActiveTag->currentLine->next;
            if (line->next)
                line->next->prev = line;
            currentActiveTag->currentLine->next = line1->next;
        }

        Token *word = line->word;
        while (word->next)
        {
            word = word->next;
        }

        word->next = currentActiveTag->currentWord->next;
        if (currentActiveTag->currentWord->next)
            currentActiveTag->currentWord->next->prev = word;
        currentActiveTag->currentWord = word;
        currentActiveTag->startIndex = word->len - offRight;

        if (currentActiveTag->startIndex < 0)
            currentActiveTag->startIndex = 0;
        if (currentActiveTag->startIndex > currentActiveTag->currentWord->len)
            currentActiveTag->currentWord->len;

        free(old->content);
        SDL_DestroyTexture(old->t1);
        free(old);
    }
    // Render as plain text
    else
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, strlen(currentActiveTag->currentWord->content), (SDL_Color){255, 255, 255, 255});
        currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(currentActiveTag->currentWord->t1, SDL_SCALEMODE_NEAREST);

        SDL_DestroySurface(s1);
        currentActiveTag->minimapDirty = 1;
    }
}

void replaceWord(char *s)
{
    if (!(currentActiveTag && currentActiveTag->currentLine && currentActiveTag->currentWord))
    {
        return;
    }
    int len = (int)strlen(s);
    if (len)
    {
        currentActiveTag->currentWord->content = s;
        currentActiveTag->currentWord->len = len;
        currentActiveTag->startIndex = len;

        // Convert edited text to tokens
        FileLine *line1 = parseText(currentActiveTag->currentWord->content);

        if (line1)
        {
            Token *old = currentActiveTag->currentWord;
            if (line1->word)
            {
                if (old->prev)
                    old->prev->next = line1->word;
                else
                    currentActiveTag->currentLine->word = line1->word;
                line1->word->next = old->next;
                line1->word->prev = old->prev;
            }

            currentActiveTag->currentWord = line1->word;

            SDL_DestroyTexture(old->t1);
            free(old);
        }
        // Render as plain text
        else
        {
            SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, currentActiveTag->currentWord->content, strlen(currentActiveTag->currentWord->content), (SDL_Color){255, 255, 255, 255});
            currentActiveTag->currentWord->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            // SDL_SetTextureScaleMode(currentActiveTag->currentWord->t1, SDL_SCALEMODE_NEAREST);

            SDL_DestroySurface(s1);
        }
    }
    currentActiveTag->minimapDirty = 1;
}

void handleEvents(SDL_Event event, int* dirty, int* running)
{
    int i = 0;
    *dirty = 1;
    if (event.type == SDL_EVENT_QUIT)
    {
        *running = 0;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        int x = (int)event.button.x;
        int y = (int)event.button.y;

        MOUSE_X = x;
        MOUSE_Y = y;
        IS_MOUSE_DOWN = 1;

        // Menu Buttons
        if (x > 0 && x < MENU_W)
        {

            // Open Folder button in Exploror
            if (!IS_TOPNAV_MENU_DOWN && !Folder && menu_state == 0 && Explorer.t3 && x > Explorer.r3.x && x < Explorer.r3.x + Explorer.r3.w && y > Explorer.r3.y && y < Explorer.r3.y + Explorer.r3.h)
            {
                // printf("Opening folder dialog...\n");
                // printf("%d\n", IS_TOPNAV_MENU_DOWN);
                open_folder_dialog();
            }

            // Files and Folders in Exploror
            if (!IS_TOPNAV_MENU_DOWN && Folder)
            {

                handleExplorerItemsClick(&Folder, x, y);
            }
        }

        // Top Nav Bar Buttons

        IS_TOPNAV_MENU_DOWN = 0;

        for (i = 0; i < (int)(sizeof(TOPNAV_MENU) / sizeof(TOPNAV_MENU[0])); i++)
        {
            TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];
            SDL_FRect r;
            r.x = node->rect.x - 7.9;
            r.w = node->rect.w + 16.8;
            r.y = node->rect.y;
            r.h = node->rect.h;
            if (x > r.x && x < r.x + r.w && y > r.y && y < r.y + r.h)
            {
                if (node->clicked)
                    node->clicked = 0;
                else
                    node->clicked = 1;
                IS_TOPNAV_MENU_DOWN = 1;
            }
            else
            {
                node->clicked = 0;
            }
        }

        // Menu Bar Buttons
        if (x < MENU_W)
        {
            for (i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
            {
                MENU_BAR_NODE *node = &LEFT_MENU[i];

                SDL_FRect r;
                r.x = node->rect.x;
                r.w = node->rect.w;
                r.y = node->rect.y - 10;
                r.h = node->rect.h + 20;

                if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                {
                    if (menu_state == i)
                    {
                        if (showMenu == 0)
                        {
                            showMenu = 1;
                            logoRect2.x = MENU_W + (WINDOW_W - MENU_W) / 2 - dlw2 / 2;
                            FILEBAR_bg_rect.x = MENU_W + 1;
                        }
                        else
                        {
                            showMenu = 0;
                            logoRect2.x = (WINDOW_W) / 2 - dlw2 / 2;
                            FILEBAR_bg_rect.x = 1;
                        }
                    }
                    else
                    {
                        showMenu = 1;
                        FILEBAR_bg_rect.x = MENU_W;
                        logoRect2.x = MENU_W + (WINDOW_W - MENU_W) / 2 - dlw2 / 2;
                    }
                    menu_state = i;
                }
            }
        }

        // TOPNAV_RIGHT Buttons
        if (y < TOPNAV_H + 50)
        {
            for (i = 0; i < (int)(sizeof(TOPNAV_RIGHT) / sizeof(TOPNAV_RIGHT[0])); i++)
            {
                MENU_BAR_NODE *node = &TOPNAV_RIGHT[i];

                SDL_FRect r;
                r.x = node->rect.x;
                r.w = node->rect.w;
                r.y = node->rect.y - 10;
                r.h = node->rect.h + 20;

                if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                {
                    node->clicked = !node->clicked;
                    if (i == 0)
                    {
                        if (node->clicked)
                            RIGHTPANEL_X = WINDOW_W - RIGHTPANEL_W;
                        else
                            RIGHTPANEL_X = WINDOW_W;
                    }
                }
                else if (!node->clicked)
                {
                    node->clicked = 0;
                }
            }
        }

        // File Bar
        if (FileBar && x > MENU_W && x < WINDOW_W && y > TOPNAV_H && y < 2 * TOPNAV_H)
        {
            FileBarItem *node = FileBar;

            while (node)
            {
                int x1 = node->r1.x;
                int x2 = x1 + node->r1.w;
                int y1 = node->r1.y;
                int y2 = y1 + node->r1.h;
                if (x > x1 && x < x2 && y > y1 && y < y2)
                {
                    node->active = 1;
                    currentActiveTag = node;
                }
                else
                {
                    node->active = 0;
                }
                node = node->next;
            }
        }

        // Text Editor Lines
        if (currentActiveTag && x > MENU_W && x < MENU_W + 70 && y > 2 * TOPNAV_H && y < WINDOW_H - FOOTER_H)
        {
            // int ty = 0;
            int cy = y - 2 * TOPNAV_H;

            FileLine *node = currentActiveTag->visibleLine;
            if (node)
            {
                int offset = cy / EDITOR_FONT_HEIGHT;
                // printf("%d\n", offset);
                while (offset > 0 && node->next)
                {
                    if (node->collapsed == 1)
                    {
                        int n = 1;
                        int exit = 0;
                        while (!exit)
                        {
                            node = node->next;
                            Token *w = node->word;
                            while (w)
                            {
                                if (strcmp(w->content, "{") == 0)
                                {
                                    n += 1;
                                }

                                if (strcmp(w->content, "}") == 0)
                                {
                                    n -= 1;
                                }

                                if (n == 0)
                                {
                                    exit = 1;
                                    break;
                                }
                                w = w->next;
                            }
                        }
                    }
                    node = node->next;
                    offset--;
                }

                Token *word = node->word;
                int cond = 0;
                int cond2 = 0;
                while (word)
                {
                    if (strcmp(word->content, "{") == 0)
                    {
                        cond++;
                        cond2 = 1;
                    }
                    if (strcmp(word->content, "}") == 0)
                    {
                        if (cond2)
                            cond--;
                    }
                    word = word->next;
                }

                if (cond > 0)
                {
                    node->collapsed = !node->collapsed;
                    int a = node->collapsed;
                    int n = 1;
                    int exit = 0;
                    while (!exit && node->next)
                    {
                        node = node->next;
                        Token *w = node->word;
                        while (w)
                        {
                            if (strcmp(w->content, "{") == 0)
                            {
                                n += 1;
                            }

                            if (strcmp(w->content, "}") == 0)
                            {
                                n -= 1;
                            }

                            if (n == 0)
                            {
                                exit = 1;
                                break;
                            }
                            w = w->next;
                        }
                    }
                    node->collapsed = a ? 2 : 0;
                }
            }
        }

        // Text Editor
        if (currentActiveTag && x > MENU_W + 70 && x < WINDOW_W - MINIMAP_W && y > 2 * TOPNAV_H && y < WINDOW_H - FOOTER_H)
        {
            IS_SELECTING = 1;
            int scrollX = currentActiveTag->EDITOR_SCROLL_X;
            // int scrollY = currentActiveTag->EDITOR_SCROLL_Y;

            // int ty = 0;
            int cy = y - 2 * TOPNAV_H;

            // if (scrollY < -EDITOR_FONT_HEIGHT)
            //     currentActiveTag->visibleLine = currentActiveTag->lines;

            FileLine *node = currentActiveTag->visibleLine;
            if (node)
            {
                int offset = cy / EDITOR_FONT_HEIGHT;
                // printf("%d\n", offset);
                while (offset > 0 && node->next)
                {

                    if (node->collapsed == 1)
                    {
                        int n = 1;
                        int exit = 0;
                        while (!exit)
                        {
                            node = node->next;
                            Token *w = node->word;
                            while (w)
                            {
                                if (strcmp(w->content, "{") == 0)
                                {
                                    n += 1;
                                }

                                if (strcmp(w->content, "}") == 0)
                                {
                                    n -= 1;
                                }

                                if (n == 0)
                                {
                                    exit = 1;
                                    break;
                                }
                                w = w->next;
                            }
                        }
                    }
                    node = node->next;
                    offset--;
                }

                currentActiveTag->currentLine = node;

                Token *word = node->word;
                int sum = scrollX + cursor->w;

                while (sum < x - MENU_W - 70 && word->next)
                {
                    if (sum + word->len * cursor->w >= x - MENU_W - 70)
                    {
                        break;
                    }
                    sum += word->len * cursor->w;
                    word = word->next;
                }
                // word = word;
                // sum += word->len * cursor->w;

                currentActiveTag->currentWord = word;
                currentActiveTag->startIndex = (x - (sum + MENU_W + 66)) / cursor->w;

                if (currentActiveTag->startIndex > word->len)
                    currentActiveTag->startIndex = word->len;
                if (currentActiveTag->startIndex < 0)
                    currentActiveTag->startIndex = 0;

                currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                // printf("%s %s %d\n", currentActiveTag->currentLine->word->content, currentActiveTag->currentWord->content, currentActiveTag->startIndex);
            }
            else
            {
                // printf("Node Does not Exist\n");
            }
        }

        // Completion Box
        if (currentActiveTag)
        {
            showCompletion = 0;
        }
    }

    if (event.type == SDL_EVENT_WINDOW_RESIZED)
    {
        // if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        // {
        WINDOW_W = event.window.data1;
        WINDOW_H = event.window.data2;

        MENU_W = WINDOW_W / 6;
        FILEBAR_bg_rect.w = MENU_W;
        FILEBAR_bg_rect.h = WINDOW_H;

        // Creating Top Nav and Left Menu
        TOPNAV_bg_rect.x = 0;
        TOPNAV_bg_rect.y = 0;
        TOPNAV_bg_rect.w = WINDOW_W;
        TOPNAV_bg_rect.h = TOPNAV_H;

        MENUBAR_bg_rect.x = 0;
        MENUBAR_bg_rect.y = TOPNAV_H;
        MENUBAR_bg_rect.w = 0;
        MENUBAR_bg_rect.h = WINDOW_H;

        MENU_bg_rect.x = 0;
        MENU_bg_rect.y = TOPNAV_H;
        MENU_bg_rect.w = MENU_W;
        MENU_bg_rect.h = WINDOW_H;

        if (showMenu)
            FILEBAR_bg_rect.x = MENU_W;
        else
            FILEBAR_bg_rect.x = 0;
        FILEBAR_bg_rect.y = TOPNAV_H;
        FILEBAR_bg_rect.w = WINDOW_W;
        FILEBAR_bg_rect.h = TOPNAV_H;

        // BG Rect
        bgRect.x = MENU_W;
        bgRect.w = (bgImageSurface->w + 0.0) / bgImageSurface->h * WINDOW_H;
        bgRect.h = WINDOW_H;

        // Full Screen Rect
        fullScreenRect.w = WINDOW_W;
        fullScreenRect.h = WINDOW_H;

        // TOPNAV_RIGHT BUTTONS

        int prevX = 0;
        for (int i = 0; i < (int)(sizeof(TOPNAV_RIGHT) / sizeof(TOPNAV_RIGHT[0])); i++)
            TOPNAV_RIGHT[0].rect.x = WINDOW_W - TOPNAV_RIGHT[0].rect.w - TOPNAV_H / 2 - (prevX++);

        // Explorer Menu
        Explorer.r1.x = 0;
        Explorer.r2.x = 0;
        Explorer.r3.x = 0;
        Explorer.r4.x = 0;

        // Search Menu
        Search.r1.x = 0;
        Search.r2.x = 0;
        Search.r3.x = 0;
        Search.r4.x = 0;

        // Github Menu
        Github.r1.x = 0;
        Github.r2.x = 0;
        Github.r3.x = 0;
        Github.r4.x = 0;

        // Github Menu
        Footer.r1.x = 0;
        Footer.r2.x = 0;
        Footer.r3.x = 0;
        Footer.r4.x = 0;
        // }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        IS_MOUSE_DOWN = 0;
        IS_SELECTING = 0;
    }

    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {

        int x = (int)event.motion.x;
        int y = (int)event.motion.y;

        MOUSE_X = x;
        MOUSE_Y = y;

        // Top Nav Left Buttons
        if (y < TOPNAV_H + 50)
        {
            for (i = 0; i < 5; i++)
            {
                TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];

                SDL_FRect r;
                r.x = node->rect.x - 7.9;
                r.w = node->rect.w + 16.8;
                r.y = node->rect.y;
                r.h = node->rect.h;

                if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                {
                    node->isActive = 1;
                }
                else if (!node->clicked)
                {
                    node->isActive = 0;
                }
            }
        }

        // TOPNAV_RIGHT Buttons
        if (y < TOPNAV_H + 50)
        {
            for (i = 0; i < (int)(sizeof(TOPNAV_RIGHT) / sizeof(TOPNAV_RIGHT[0])); i++)
            {
                MENU_BAR_NODE *node = &TOPNAV_RIGHT[i];

                SDL_FRect r;
                r.x = node->rect.x;
                r.w = node->rect.w;
                r.y = node->rect.y - 10;
                r.h = node->rect.h + 20;

                if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                {
                    node->isActive = 1;
                }
                else
                {
                    node->isActive = 0;
                }
            }
        }

        // Menu Bar Buttons
        if (x < MENU_W)
        {
            for (i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
            {
                MENU_BAR_NODE *node = &LEFT_MENU[i];

                SDL_FRect r;
                r.x = node->rect.x;
                r.w = node->rect.w;
                r.y = node->rect.y - 10;
                r.h = node->rect.h + 20;

                if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                {
                    node->isActive = 1;
                }
                else if (!node->clicked)
                {
                    node->isActive = 0;
                }
            }
        }

        // Files and Folders in Exploror
        if (x > 0 && x < MENU_W + 150)
        {
            if (Folder)
            {

                handleExplorerItemsHover(&Folder, x, y);
            }
        }

        // Text Editor
        if (currentActiveTag && currentActiveTag->type != AETHER_IMG && x > MENU_W + 70 && x < WINDOW_W - MINIMAP_W && y > 2 * TOPNAV_H && y < WINDOW_H - FOOTER_H)
        {
            SDL_SetCursor(ibeam);
        }
        else
        {
            SDL_SetCursor(arrowCursor);
        }

        // Handle Selection
        if (IS_SELECTING)
        {
            currentActiveTag->SELECTION_START_INDEX = (x - MENU_W - 70) / cursor->w;

            int scrollX = currentActiveTag->EDITOR_SCROLL_X;
            // int scrollY = currentActiveTag->EDITOR_SCROLL_Y;

            // int ty = 0;
            int cy = y - 2 * TOPNAV_H;

            // if (scrollY < -EDITOR_FONT_HEIGHT)
            //     currentActiveTag->visibleLine = currentActiveTag->lines;

            FileLine *node = currentActiveTag->visibleLine;
            if (node)
            {
                int offset = cy / EDITOR_FONT_HEIGHT;
                // printf("%d\n", offset);
                while (offset > 0 && node->next)
                {
                    node = node->next;
                    offset--;
                }

                currentActiveTag->currentLine = node;

                Token *word = node->word;
                int sum = scrollX + cursor->w;

                while (sum < x - MENU_W - 70 && word->next)
                {
                    if (sum + word->len * cursor->w >= x - MENU_W - 70)
                    {
                        break;
                    }
                    sum += word->len * cursor->w;
                    word = word->next;
                }
                // word = word;
                // sum += word->len * cursor->w;

                currentActiveTag->currentWord = word;
                currentActiveTag->startIndex = (x - (sum + MENU_W + 70)) / cursor->w;

                if (currentActiveTag->startIndex > word->len)
                    currentActiveTag->startIndex = word->len;
                if (currentActiveTag->startIndex < 0)
                    currentActiveTag->startIndex = 0;
                // printf("%s %s %d\n", currentActiveTag->currentLine->word->content, currentActiveTag->currentWord->content, currentActiveTag->startIndex);
            }
            else
            {
                // printf("Node Does not Exist\n");
            }
        }
    }

    if (event.type == SDL_EVENT_MOUSE_WHEEL)
    {
        if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
        {
            handleMouseScroll(-event.wheel.x, -event.wheel.y);
        }
        else
        {
            handleMouseScroll(event.wheel.x, event.wheel.y);
        }
    }

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        SDL_Keycode key = event.key.key;
        SDL_Keymod mod = event.key.mod;

        if (currentActiveTag)
        {
            if (key == SDLK_BACKSPACE)
            {
                leftDeleteChar();
                currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                showCompletion = 0;

                if (currentActiveTag->currentLine->collapsed == 2)
                {
                    currentActiveTag->currentLine->collapsed = 0;

                    int cond = 1;
                    FileLine *line = currentActiveTag->currentLine;
                    while (cond != 0)
                    {

                        line = line->prev;
                        if (line->collapsed == 2)
                            cond++;
                        if (line->collapsed == 1)
                            cond--;
                    }

                    line->collapsed = 0;
                }
            }

            if (key == SDLK_UP)
            {
                if (CompletionBox.active && showCompletion)
                {
                    if (CompletionBox.active->prev)
                        CompletionBox.active = CompletionBox.active->prev;
                    else
                        CompletionBox.active = CompletionBox.tail;
                }
                else
                {
                    moveCursorUp();

                    currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                    currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                    currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                    showCompletion = 0;
                }
            }

            if (key == SDLK_DOWN)
            {
                if (showCompletion && CompletionBox.active)
                {
                    if (CompletionBox.active->next)
                        CompletionBox.active = CompletionBox.active->next;
                    else
                        CompletionBox.active = CompletionBox.list;
                }
                else
                {
                    moveCursorDown();

                    currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                    currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                    currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                    showCompletion = 0;
                }
            }

            if (key == SDLK_LEFT)
            {
                if ((mod & SDL_KMOD_CTRL))
                {
                    if (currentActiveTag->startIndex == 0)
                    {
                        if (currentActiveTag->currentWord && currentActiveTag->currentWord->prev)
                            currentActiveTag->currentWord = currentActiveTag->currentWord->prev;
                    }
                    currentActiveTag->startIndex = 0;
                }
                else
                    moveCursorLeft();

                currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                if (showCompletion)
                    getCompletion(currentActiveTag->currentWord->content, currentActiveTag->startIndex);
            }

            if (key == SDLK_RIGHT)
            {
                if ((mod & SDL_KMOD_CTRL))
                {
                    if (currentActiveTag->startIndex == currentActiveTag->currentWord->len)
                    {
                        if (currentActiveTag->currentWord && currentActiveTag->currentWord->next)
                            currentActiveTag->currentWord = currentActiveTag->currentWord->next;
                    }
                    currentActiveTag->startIndex = currentActiveTag->currentWord->len;
                }
                else
                    moveCursorRight();

                currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                if (showCompletion)
                    getCompletion(currentActiveTag->currentWord->content, currentActiveTag->startIndex);
            }

            if (key == SDLK_HOME)
            {
                if (mod & SDL_KMOD_CTRL)
                {
                    currentActiveTag->currentLine = currentActiveTag->lines;
                    currentActiveTag->currentWord = currentActiveTag->currentLine->word;
                    currentActiveTag->EDITOR_SCROLL_Y = 0;
                }
                if (currentActiveTag->currentLine->word)
                    currentActiveTag->currentWord = currentActiveTag->currentLine->word;
                currentActiveTag->startIndex = 0;

                currentActiveTag->EDITOR_SCROLL_X = 0;

                currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
            }

            if (key == SDLK_END)
            {

                if (mod & SDL_KMOD_CTRL)
                {
                    FileLine *line = currentActiveTag->currentLine;
                    int y = 0;
                    while (line->next)
                    {
                        y++;
                        currentActiveTag->EDITOR_SCROLL_Y -= cursor->h;
                        line = line->next;
                    }
                    if (y * cursor->h > WINDOW_H)
                        currentActiveTag->EDITOR_SCROLL_Y += WINDOW_H;

                    currentActiveTag->currentLine = line;
                    currentActiveTag->currentWord = line->word;
                }

                Token *word = currentActiveTag->currentWord;
                while (word->next)
                {
                    word = word->next;
                }

                currentActiveTag->currentWord = word;
                currentActiveTag->startIndex = word->len;

                currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
            }

            if (key == SDLK_RETURN)
            {
                if (showCompletion)
                {
                    replaceWord(CompletionBox.active->text);
                }
                else
                {
                    createNewline();
                }

                currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                showCompletion = 0;
            }

            if ((mod & SDL_KMOD_CTRL) && key == SDLK_W)
            {
                removeTab(currentActiveTag);
            }

            if ((mod & SDL_KMOD_CTRL) && key == SDLK_TAB)
            {
                if (mod & SDL_KMOD_SHIFT)
                {
                    if (currentActiveTag->prev)
                        currentActiveTag = currentActiveTag->prev;
                    else
                        currentActiveTag = FileBar;
                }
                else
                {
                    if (currentActiveTag->next)
                        currentActiveTag = currentActiveTag->next;
                    else
                        currentActiveTag = FileBar;
                }
            }

            else
            {
                if (key == SDLK_TAB)
                {
                    if (showCompletion)
                    {
                        replaceWord(CompletionBox.active->text);
                    }
                    else
                    {
                        insertString("    ");
                    }

                    currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                    currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                    currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                    showCompletion = 0;
                }
            }

            if ((mod & SDL_KMOD_CTRL) && key == SDLK_S)
            {
                char *content = convertToText(currentActiveTag);
                writeFile(currentActiveTag->path, content);
            }

            if ((mod & SDL_KMOD_CTRL) && key == SDLK_V)
            {
                if (SDL_HasClipboardText())
                {
                    char *s = SDL_GetClipboardText();
                    insertString(s);
                    free(s);
                    // SDL_free(s) in SDL3 usually, but free() might work if aligned.
                    // SDL3 recommends SDL_free.

                    currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                    currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                    currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                }
            }
        }
    }

    if (event.type == SDL_EVENT_TEXT_INPUT)
    {
        if (currentActiveTag)
        {
            // insertChar(event.text.text[0]);
            if (currentActiveTag->currentLine->collapsed == 1)
            {
                currentActiveTag->currentLine->collapsed = 0;

                FileLine *line = currentActiveTag->currentLine;
                int n = 1;
                while (n != 0)
                {
                    line = line->next;
                    if (line->collapsed == 1)
                    {
                        n += 1;
                    }
                    if (line->collapsed == 2)
                    {
                        n -= 1;
                    }
                }
                line->collapsed = 0;
            }

            int disableCompletion = 0;

            if (strcmp(event.text.text, "}") == 0 && currentActiveTag->currentWord->next && strcmp(currentActiveTag->currentWord->next->content, "}") == 0)
            {
                currentActiveTag->currentWord = currentActiveTag->currentWord->next;
                currentActiveTag->startIndex = 1;

                disableCompletion = 1;
            }
            else if (strcmp(event.text.text, ")") == 0 && currentActiveTag->currentWord->next && strcmp(currentActiveTag->currentWord->next->content, ")") == 0)
            {
                currentActiveTag->currentWord = currentActiveTag->currentWord->next;
                currentActiveTag->startIndex = 1;

                disableCompletion = 1;
            }
            else
            {
                insertString(event.text.text);
                if (strcmp(event.text.text, "{") == 0)
                {
                    insertString("}");
                    if (currentActiveTag->currentWord && currentActiveTag->currentWord->prev)
                    {
                        currentActiveTag->currentWord = currentActiveTag->currentWord->prev;
                        currentActiveTag->startIndex = 1;
                    }
                    disableCompletion = 1;
                    showCompletion = 0;
                }

                if (strcmp(event.text.text, "(") == 0)
                {
                    insertString(")");
                    if (currentActiveTag->currentWord && currentActiveTag->currentWord->prev)
                    {
                        currentActiveTag->currentWord = currentActiveTag->currentWord->prev;
                        currentActiveTag->startIndex = 1;
                    }
                    disableCompletion = 1;
                    showCompletion = 0;
                }

                if ((currentActiveTag->startIndex == 0 || currentActiveTag->startIndex == currentActiveTag->currentWord->len) && strcmp(event.text.text, "'") == 0)
                {
                    insertString("'");
                    currentActiveTag->startIndex -= 1;
                    disableCompletion = 1;
                    showCompletion = 0;
                }

                if ((currentActiveTag->startIndex == 0 || currentActiveTag->startIndex == currentActiveTag->currentWord->len) && strcmp(event.text.text, "\"") == 0)
                {
                    insertString("\"");
                    currentActiveTag->startIndex -= 1;
                    disableCompletion = 1;
                    showCompletion = 0;
                }
            }
            currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
            currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
            currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

            if (!disableCompletion)
                getCompletion(currentActiveTag->currentWord->content, currentActiveTag->startIndex);
        }
    }
}