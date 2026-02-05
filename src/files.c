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
FileNode* Folder = NULL;

/**
 * @brief Opens a native folder selection dialog
 * 
 * Uses Zenity (on Linux) to display a system folder selection dialog.
 * This function blocks until the user selects a folder or cancels.
 * 
 * @return Pointer to selected folder path, or NULL if cancelled/error
 * @note The returned string is static and will be overwritten on next call
 */
char* open_folder_dialog() {
    FILE *fp;
    static char path[1024];

    // Execute zenity file selection dialog
    fp = popen("zenity --file-selection --directory --title=\"Select a folder\"", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run zenity\n");
        return NULL;
    }

    // Read the selected path from zenity output
    if (fgets(path, sizeof(path), fp) == NULL) {
        pclose(fp);
        return NULL;
    }

    // Remove trailing newline character from path
    path[strcspn(path, "\n")] = '\0';

    pclose(fp);
    return path;
}

/**
 * @brief Thread function for non-blocking folder selection
 * 
 * This function runs in a separate thread to prevent the UI from freezing
 * while the folder dialog is open. Updates the global folder state and
 * reinitializes the file explorer when a folder is selected.
 * 
 * @param arg Unused thread argument
 * @return NULL (required by pthread interface)
 */
void* open_folder_thread() {
    char* folder = open_folder_dialog();
    if (folder) {
        printf("You selected: %s\n", folder);
        // Copy selected path to global state
        strncpy(selected_folder, folder, sizeof(selected_folder));
        // Reinitialize file explorer with new folder
        initExplorer();
    } else {
        printf("No folder selected.\n");
    }
    return NULL;
}

/**
 * @brief Clears the file explorer and frees all associated memory
 * 
 * Traverses the entire file tree and deallocates all FileNode structures.
 * Should be called before loading a new folder or when shutting down.
 */
void clearExplorer(){
    FileNode* temp = Folder;
    while (temp!=NULL)
    {
        FileNode* ptr = temp;
        temp = temp->next;
        free(ptr);
    }
    Folder = NULL;
}

void initExplorer(){
    if(strlen(selected_folder) == 0) return;
    
    DIR* dir = opendir(selected_folder);
    if(!dir){
        perror("opendir");
        return;
    }

    struct dirent* entry;
    char fullpath[2048];
    struct stat st;
    int i=0;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if(entry->d_name[0] == '.') continue;

        // printf("%s ", entry->d_name);
        int type = 0;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", selected_folder, entry->d_name);
        
        if (stat(fullpath, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                type = 1;
            } else if (S_ISREG(st.st_mode)) {
                type = 0;
            } else {
                type = 0;
            }
        } else {
            perror("stat");
        }

        FileNode* node = createFileNode(entry->d_name, fullpath, type);

        node->opened = 0;
        node->next = Folder;
        if(Folder) Folder->prev = node;
        
        SDL_Color color = {255, 255, 255, 255};
        if (!poppins_regular) {
            fprintf(stderr, "Error: poppins_regular font not loaded\n");
            continue;
        }
        SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, node->name, strlen(node->name), color);
        if (!s1) {
            fprintf(stderr, "Error creating text surface for %s: %s\n", node->name, SDL_GetError());
            continue;
        }
        

        node->r1.x = LEFT_MENU[0].rect.x + s1->h + 2;
        node->r1.w = s1->w;
        node->r1.y = LEFT_MENU[0].rect.y + LEFT_MENU[0].rect.h + s1->h*i + MENU_PAD_Y/2*i;
        node->r1.h = s1->h;

        SDL_DestroySurface(s1);
        
        i+=1;
        Folder = node;
    }

    FileNode* parent = createFileNode("FOLDER", selected_folder, 1);
    parent->opened = 1;
    parent->child = Folder;
    parent->isDirOpened = 1;
    Folder = parent;
    
    SDL_Color color = {255, 255, 255, 255};
    if (!poppins_regular) {
        fprintf(stderr, "Error: poppins_regular font not loaded\n");
        return;
    }
    SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, parent->name, strlen(parent->name), color);
    if (!s1) {
        fprintf(stderr, "Error creating text surface for parent: %s\n", SDL_GetError());
        return;
    }

    parent->r1.x = LEFT_MENU[0].rect.x;
    parent->r1.w = s1->w;
    parent->r1.y = LEFT_MENU[0].rect.y + MENU_BAR_H - MENU_PAD_Y;
    parent->r1.h = s1->h;

    SDL_DestroySurface(s1);

    closedir(dir);
}

char* readFile(char* path){
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return NULL;
    }
    
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);
    
    char* content = malloc(len+1);
    if (!content) {
        fprintf(stderr, "Error: Could not allocate memory for file content\n");
        fclose(f);
        return NULL;
    }
    
    fread(content, 1, len, f);
    content[len] = '\0';
    fclose(f);
    return content;
}


void writeFile(const char* path, const char* content){
    FILE* f = fopen(path, "w");
    if(!f){
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return;
    }

    fprintf(f, "%s", content);
    fclose(f);
}