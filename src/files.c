/**
 * @file files.c
 * @brief File system operations and file explorer functionality
 *
 * This module provides:
 * - File and directory browsing capabilities
 * - File I/O operations (read, write, create)
 * - Folder selection dialogs using native OS dialogs
 * - File tree management and navigation
 * - Asynchronous file operations to prevent UI blocking
 */

#include "../include/files.h"

// ======== GLOBAL FILE STATE ========

/** @brief Path to currently selected folder */
char selected_folder[1024] = "";

/** @brief Root node of the file explorer tree */
FileNode *Folder = NULL;

/**
 * @brief Opens a native folder selection dialog
 *
 * Uses SDL3 to display a system folder selection dialog.
 * This function blocks until the user selects a folder or cancels.
 *
 * @return Pointer to selected folder path, or NULL if cancelled/error
 * @note The returned string is static and will be overwritten on next call
 */
void folder_callback(void *userdata, const char *const *filelist, int filter)
{
    if (!filelist)
    {
        printf("No folder selected.\n");
        return;
    }

    const char *folder = filelist[0];
    // printf("You selected: %s\n", folder);

    // Copy selected path to global state
    snprintf(selected_folder, sizeof(selected_folder), "%s", folder);

    // Reinitialize file explorer with new folder
    initExplorer();
}

void open_folder_dialog()
{
    SDL_ShowOpenFolderDialog(folder_callback, NULL, window, NULL, false);
}

/**
 * @brief Clears the file explorer and frees all associated memory
 *
 * Traverses the entire file tree and deallocates all FileNode structures.
 * Should be called before loading a new folder or when shutting down.
 */
void clearExplorer()
{
    FileNode *temp = Folder;
    while (temp != NULL)
    {
        FileNode *ptr = temp;
        temp = temp->next;
        free(ptr);
    }
    Folder = NULL;
}

SDL_EnumerationResult explorer_callback(void *userdata, const char *dirname, const char *fname)
{
    
    if (!fname)
        return SDL_ENUM_CONTINUE;

    if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0)
        return SDL_ENUM_CONTINUE;

    if (fname[0] == '.')
        return SDL_ENUM_CONTINUE;

    char fullpath[2048];
    snprintf(fullpath, sizeof(fullpath), "%s%s", dirname, fname);

    SDL_PathInfo info;

    if (!SDL_GetPathInfo(fullpath, &info)) {
        printf("PathInfo failed for: %s\n", fullpath);
        printf("SDL Error: %s\n", SDL_GetError());
        return SDL_ENUM_CONTINUE;
    }

    int type = (info.type == SDL_PATHTYPE_DIRECTORY) ? 1 : 0;

    FileNode *node = createFileNode(fname, fullpath, type);
    
    node->opened = 0;
    node->next = Folder;
    if (Folder)
        Folder->prev = node;

    SDL_Color color = {255, 255, 255, 255};
    if (!poppins_regular)
    {
        fprintf(stderr, "Error: poppins_regular font not loaded\n");
    }
    SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->name, strlen(node->name), color);
    if (!s1)
    {
        fprintf(stderr, "Error creating text surface for %s: %s\n", node->name, SDL_GetError());
    }

    node->r1.x = LEFT_MENU[0].rect.x + s1->h + 2;
    node->r1.w = s1->w;
    // node->r1.y = LEFT_MENU[0].rect.y + LEFT_MENU[0].rect.h + s1->h * i + MENU_PAD_Y / 2 * i;
    if(Folder) node->r1.y = Folder->r1.y + s1->h + MENU_PAD_Y / 2;
    else node->r1.y = LEFT_MENU[0].rect.y + LEFT_MENU[0].rect.h;
    node->r1.h = s1->h;

    SDL_DestroySurface(s1);


    Folder = node;
    return SDL_ENUM_CONTINUE;
}

void initExplorer()
{
    if (strlen(selected_folder) == 0)
        return;

    Folder = NULL;

    SDL_EnumerateDirectory(selected_folder, explorer_callback, NULL);


    FileNode *parent = createFileNode("FOLDER", selected_folder, 1);
    parent->opened = 1;
    parent->child = Folder;
    parent->isDirOpened = 1;
    Folder = parent;

    SDL_Color color = {255, 255, 255, 255};
    if (!poppins_regular)
    {
        fprintf(stderr, "Error: poppins_regular font not loaded\n");
        return;
    }
    SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, parent->name, strlen(parent->name), color);
    if (!s1)
    {
        fprintf(stderr, "Error creating text surface for parent: %s\n", SDL_GetError());
        return;
    }

    parent->r1.x = LEFT_MENU[0].rect.x;
    parent->r1.w = s1->w;
    parent->r1.y = LEFT_MENU[0].rect.y + MENU_BAR_H - MENU_PAD_Y;
    parent->r1.h = s1->h;

    SDL_DestroySurface(s1);
}

SDL_EnumerationResult populate_callback(void *userdata, const char *dirname, const char *fname)
{
    if (!fname)
        return SDL_ENUM_CONTINUE;

    if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0)
        return SDL_ENUM_CONTINUE;

    if (fname[0] == '.')
        return SDL_ENUM_CONTINUE;

    char fullpath[2048];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", dirname, fname);

    SDL_PathInfo info;
    if (!SDL_GetPathInfo(fullpath, &info))
    {
        return SDL_ENUM_CONTINUE;
    }

    int type = (info.type == SDL_PATHTYPE_DIRECTORY) ? 1 : 0;

    FileNode *parent = (FileNode *)userdata;

    FileNode *node = createFileNode(fname, fullpath, type);
    node->opened = 0;

    node->next = parent->child;
    if (parent->child)
        parent->child->prev = node;
    parent->child = node;

    return SDL_ENUM_CONTINUE;
}

void populateFolder(FileNode *folder)
{
    if (!folder)
        return;
    if (folder->child)
        return;

    SDL_EnumerateDirectory(folder->path, populate_callback, folder);
    folder->isDirOpened = 1;
}

char *readFile(char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);

    char *content = malloc(len + 1);
    if (!content)
    {
        fprintf(stderr, "Error: Could not allocate memory for file content\n");
        fclose(f);
        return NULL;
    }

    fread(content, 1, len, f);
    content[len] = '\0';
    fclose(f);
    return content;
}

void writeFile(const char *path, const char *content)
{
    FILE *f = fopen(path, "w");
    if (!f)
    {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return;
    }

    fprintf(f, "%s", content);
    fclose(f);
}