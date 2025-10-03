#include "../include/files.h"

char selected_folder[1024] = "";
FileNode* Folder = NULL;

char* open_folder_dialog() {
    FILE *fp;
    static char path[1024];

    fp = popen("zenity --file-selection --directory --title=\"Select a folder\"", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run zenity\n");
        return NULL;
    }

    if (fgets(path, sizeof(path), fp) == NULL) {
        pclose(fp);
        return NULL;
    }

    // Remove newline character
    path[strcspn(path, "\n")] = '\0';

    pclose(fp);
    return path;
}


void* open_folder_thread() {
    char* folder = open_folder_dialog();
    if (folder) {
        printf("You selected: %s\n", folder);
        strncpy(selected_folder, folder, sizeof(selected_folder));
        initExplorer();
    } else {
        printf("No folder selected.\n");
    }
    return NULL;
}

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
        SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, node->name, color);
        if (!s1) {
            fprintf(stderr, "Error creating text surface for %s: %s\n", node->name, TTF_GetError());
            continue;
        }
        

        node->r1.x = LEFT_MENU[0].rect.x + s1->h + 2;
        node->r1.w = s1->w;
        node->r1.y = LEFT_MENU[0].rect.y + LEFT_MENU[0].rect.h + s1->h*i + MENU_PAD_Y/2*i;
        node->r1.h = s1->h;

        SDL_FreeSurface(s1);
        
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
    SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, parent->name, color);
    if (!s1) {
        fprintf(stderr, "Error creating text surface for parent: %s\n", TTF_GetError());
        return;
    }

    parent->r1.x = LEFT_MENU[0].rect.x;
    parent->r1.w = s1->w;
    parent->r1.y = LEFT_MENU[0].rect.y + MENU_BAR_H - MENU_PAD_Y;
    parent->r1.h = s1->h;

    SDL_FreeSurface(s1);

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