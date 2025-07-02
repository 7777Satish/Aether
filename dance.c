#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    printf("Opening folder dialog...\n");
    char* folder = open_folder_dialog();
    if (folder) {
        printf("You selected: %s\n", folder);
    } else {
        printf("No folder selected.\n");
    }

    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
