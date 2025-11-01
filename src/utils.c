#include "utils.h"

FileBarItem *FileBar = NULL;
FileBarItem *currentActiveTag = NULL;
int TotalFileBarLength = 0;

FileType getFileType(char *filename)
{
    char *lastDot = NULL;
    while (*filename)
    {
        if (*filename == '.')
            lastDot = filename;
        filename++;
    }

    if (!lastDot)
        return AETHER_TXT;

    lastDot++;

    if (strcmp(lastDot, "c") == 0)
        return AETHER_CLANG;
    else if (strcmp(lastDot, "h") == 0)
        return AETHER_CHEADER;
    else if (strcmp(lastDot, "html") == 0)
        return AETHER_HTML;
    else if (strcmp(lastDot, "css") == 0)
        return AETHER_CSS;
    else if (strcmp(lastDot, "txt") == 0)
        return AETHER_TXT;
    else if (strcmp(lastDot, "png") == 0 || strcmp(lastDot, "jpg") == 0)
        return AETHER_IMG;

    return AETHER_TXT;
}

FileNode *createFileNode(char *name, char *path, int isDir)
{
    FileNode *node = (FileNode *)malloc(sizeof(FileNode));
    node->name = strdup(name);
    node->path = strdup(path);
    node->type = isDir ? AETHER_TXT : getFileType(name);
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

FileBarItem *createFileBarNode(char *name, char *path)
{
    FileBarItem *node = (FileBarItem *)malloc(sizeof(FileBarItem));
    node->name = strdup(name);
    node->path = strdup(path);
    node->type = getFileType(name);
    node->next = NULL;
    node->prev = NULL;
    node->t1 = NULL;
    node->t2 = NULL;
    node->active = 1;
    node->lines = NULL;
    node->cursorX = 0;
    node->cursorY = 0;
    node->EDITOR_SCROLL_X = 0;
    node->EDITOR_SCROLL_Y = 0;
    node->SELECTION_START_LINE = NULL;
    node->SELECTION_START_WORD = NULL;
    node->SELECTION_START_INDEX = 0;
    node->currentLine = NULL;
    node->currentWord = NULL;
    node->visibleLine = NULL;
    node->startIndex = 0;
    return node;
}

void inActiveAllFileNodes()
{
    FileBarItem *node = FileBar;
    while (node)
    {
        node->active = 0;
        node = node->next;
    }
}



void addFileBarNode(char *name, char *path)
{
    FileBarItem *node = createFileBarNode(name, path);
    node->active = 1;
    currentActiveTag = node;
    if(node->type != AETHER_IMG) {
        char *content = readFile(path);
        FileLine *lines = parseText(content);
    
        node->lines = lines;
        node->currentLine = lines;
        node->visibleLine = lines;
        node->SELECTION_START_LINE = lines;
        if (lines){
            node->currentWord = lines->word;
            node->SELECTION_START_WORD = lines->word;
        }
    }

    if (!node->t1)
    {
        SDL_Color color = {239, 239, 239, 255};
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->name, color);
        TotalFileBarLength += s1->w + 40;
        node->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        SDL_FreeSurface(s1);
    }

    if (!FileBar)
    {
        FileBar = node;
    }
    else
    {
        FileBarItem *ptr = FileBar;
        while (ptr->next != NULL)
        {
            if (strcmp(ptr->path, path) == 0)
            {
                ptr->active = 1;
                currentActiveTag = ptr;

                return;
            }
            else
            {
                ptr->active = 0;
            }
            ptr = ptr->next;
        }
        if (strcmp(ptr->path, path) == 0)
        {
            ptr->active = 1;
            currentActiveTag = ptr;

            return;
        }

        ptr->next = node;
        node->prev = ptr;
    }
}

void handleExplorerItemsHover(FileNode **folder, int x, int y)
{
    // y+=EXPLORER_SCROLL_Y;
    FileNode *node = (*folder)->child;
    while (node != NULL)
    {

        if (x > MENU_PAD_X && x < MENU_W - MENU_PAD_X && y > node->r1.y - MENU_PAD_Y / 4 && y < node->r1.y + node->r1.h + MENU_PAD_Y / 4)
        {
            node->hovered = 1;
        }
        else
        {
            node->hovered = 0;
        }

        if (node->isDir && node->child)
        {
            handleExplorerItemsHover(&node, x, y);
        }

        node = node->next;
    }
}

void handleExplorerItemsClick(FileNode **folder, int x, int y)
{
    // y+=EXPLORER_SCROLL_Y;
    FileNode *node = (*folder)->child;
    while (node != NULL)
    {
        if (x > MENU_PAD_X && x < MENU_W - MENU_PAD_X && y > node->r1.y - MENU_PAD_Y / 4 && y < node->r1.y + node->r1.h + MENU_PAD_Y / 4)
        {
            if (node->isDir)
            {
                node->opened = !node->opened;
                node->active = 1;
            }
            else
            {
                addFileBarNode(node->name, node->path);
            }
        }
        else
        {
            node->active = 0;
        }

        if (node->isDir && node->child && node->opened)
        {
            handleExplorerItemsClick(&node, x, y);
        }

        node = node->next;
    }
}