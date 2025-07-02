#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/renderer.h"
#include "files.h"

void renderFile();
char* open_folder_dialog();
void* open_folder_thread();

// ======== LEFT MENU EXPLORER ========

int editorState = 0;


int main(){

    init();

    int i = 0;

    /* ===== Initialising Images ===== */
    SDL_Surface* logoSurface = IMG_Load("assets/logo.png");
    SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);
    int logoAW, logoAH;
    SDL_QueryTexture(logoTexture, NULL, NULL, &logoAW, &logoAH);
    TOP_NAV_LOGO_W = (logoAW+0.0)/logoAH*TOP_NAV_LOGO_H;
    int dlh = TOPNAV_H - 2*TOPNAV_PADDINGY;
    int dlw = (logoAW+0.0)/logoAH*dlh;
    SDL_Rect logoRect = {MENU_BAR_W/2 - dlw/2, TOPNAV_H/2 - dlh/2, dlw, dlh};

    int running = 1;
    SDL_Event event;
    while (running)
    {
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = 0;
            }

            if(event.type == SDL_MOUSEBUTTONDOWN){

                int x = event.motion.x;
                int y = event.motion.y;
                

                // Menu Buttons
                if(x > MENU_BAR_W && x < MENU_BAR_W + MENU_W){
                    
                    // Open Folder button in Exploror
                    if(!IS_TOPNAV_MENU_DOWN && !Folder && menu_state == 0 && Explorer.t3 && x>Explorer.r3.x && x<Explorer.r3.x+Explorer.r3.w && y> Explorer.r3.y && y<Explorer.r3.y+Explorer.r3.h){
                        printf("Opening folder dialog...\n");
                        printf("%d\n", IS_TOPNAV_MENU_DOWN);
                        pthread_t folder_thread;
                        pthread_create(&folder_thread, NULL, open_folder_thread, NULL);
                        pthread_detach(folder_thread);  // optional: auto-clean thread

                    }

                    // Files and Folders in Exploror
                    if(!IS_TOPNAV_MENU_DOWN && Folder){
                        
                        handleExplorerItemsClick(&Folder, x, y);
                        
                    }

                }

                // Top Nav Bar Buttons

                IS_TOPNAV_MENU_DOWN = 0;

                for(i=0; i<(int)(sizeof(TOPNAV_MENU)/sizeof(TOPNAV_MENU[0])); i++){
                    TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];
                    SDL_Rect r;
                    r.x = node->rect.x-7.9;
                    r.w = node->rect.w+16.8;
                    r.y = node->rect.y;
                    r.h = node->rect.h;
                    if(x>r.x && x<r.x + r.w && y>r.y && y<r.y + r.h){
                        if(node->clicked) node->clicked = 0;
                        else node->clicked = 1;
                        IS_TOPNAV_MENU_DOWN = 1;
                    } else {
                        node->clicked = 0;
                    }
                }

                // Menu Bar Buttons
                if(x<MENU_BAR_W+50){
                    for(i=0;i<(int)(sizeof(LEFT_MENU)/sizeof(LEFT_MENU[0]));i++){
                        MENU_BAR_NODE *node = &LEFT_MENU[i];
                        
                        SDL_Rect r;
                        r.x = 0;
                        r.w = MENU_BAR_W;
                        r.y = node->rect.y - 10;
                        r.h = node->rect.h + 20;
    
                        if(x>r.x && x<r.w + r.x && y>r.y && y<r.y + r.h){
                            if(menu_state == i){
                                if(showMenu == 0){
                                    showMenu = 1;
                                    logoRect2.x = MENU_BAR_W + MENU_W + (WINDOW_W - MENU_BAR_W - MENU_W)/2 - dlw2/2;
                                    FILEBAR_bg_rect.x = MENU_BAR_W + MENU_W + 1;
                                }
                                else{
                                    showMenu = 0;
                                    logoRect2.x = MENU_BAR_W + (WINDOW_W-MENU_BAR_W)/2 - dlw2/2;
                                    FILEBAR_bg_rect.x = MENU_BAR_W + 1;
                                }
                            } else {
                                showMenu = 1;
                                FILEBAR_bg_rect.x = MENU_BAR_W + MENU_W;
                                logoRect2.x = MENU_BAR_W + MENU_W + (WINDOW_W - MENU_BAR_W - MENU_W)/2 - dlw2/2;
                            }
                            menu_state = i;
                        }
                    }
                }

            }

            if(event.type == SDL_MOUSEMOTION){

                int x = event.motion.x;
                int y = event.motion.y;

                // Top Nav Left Buttons
                if(y<TOPNAV_H + 50){
                    for(i=0;i<5;i++){
                        TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];
                        
                        SDL_Rect r;
                        r.x = node->rect.x-7.9;
                        r.w = node->rect.w+16.8;
                        r.y = node->rect.y;
                        r.h = node->rect.h;
    
                        if(x>r.x && x<r.w + r.x && y>r.y && y<r.y + r.h){
                            node->isActive = 1;
                        } else if(!node->clicked){
                            node->isActive = 0;
                        }
                    }
                }

                // Menu Bar Buttons
                if(x<MENU_BAR_W+50){
                    for(i=0;i<(int)(sizeof(LEFT_MENU)/sizeof(LEFT_MENU[0]));i++){
                        MENU_BAR_NODE *node = &LEFT_MENU[i];
                        
                        SDL_Rect r;
                        r.x = 0;
                        r.w = MENU_BAR_W;
                        r.y = node->rect.y - 10;
                        r.h = node->rect.h + 20;
    
                        if(x>r.x && x<r.w + r.x && y>r.y && y<r.y + r.h){
                            node->isActive = 1;
                        } else if(!node->clicked){
                            node->isActive = 0;
                        }
                    }
                }

                if(x>MENU_BAR_W && x<MENU_BAR_W + MENU_W){


                    // Files and Folders in Exploror
                    if(Folder){
                        
                        handleExplorerItemsHover(&Folder, x, y);
                        
                    }

                }
                
            }
        }

        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
        SDL_RenderClear(renderer);


        /* ===== Draw Home Screen ===== */

        renderTopNav();
       
        renderMenuBar();
        renderTextEditor();

        // Drawing Logos
        SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
        // if(editorState == 0){
        //     SDL_RenderCopy(renderer, logoTexture2, NULL, &logoRect2);
        // } else if(editorState == 1){
        //     renderFile();
        // }

        renderTopNavBarMenu();
        renderFileBar();

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void renderFile(){
    
    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);

    SDL_RenderDrawLine(renderer, MENU_BAR_W + MENU_W, TOPNAV_H + 30, WINDOW_W, TOPNAV_H + 30);
}
