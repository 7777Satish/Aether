#include "files.h"

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
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        printf("\n%s", entry->d_name);
    }

    closedir(dir);
}