#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


void renderExplorer();
void renderSearch();
void renderGithub();
void renderExtentions();


typedef struct{
    char name[10];
    char list[10][20];
    int isActive;
    int clicked;
    SDL_Texture* texture;
    SDL_Rect rect;
} TOPNAV_MENU_NODE;

typedef struct{
    char name[15];
    char icon[40];
    char active_icon[40];
    int isActive;
    int clicked;
    SDL_Texture* texture;
    SDL_Texture* active_texture;
    SDL_Texture* text_texture;
    SDL_Rect rect;
    SDL_Rect text_rect;
} MENU_BAR_NODE;


int WINDOW_W = 1200;
int WINDOW_H = 700;
// int WINDOW_W = 1200*2.5/2;
// int WINDOW_H = 700*2.5/2;

// Initialising Left Menu
int MENU_BAR_W = 50;
int MENU_W = 200;
int MENU_PAD_X = 10;
int MENU_PAD_Y = 10;
int showMenu = 1;
int menu_state = 0;

// Initialising TOPNAV
int TOPNAV_H = 33;
int TOPNAV_PADDINGX = 20;
int TOPNAV_PADDINGY = 6;
int TOP_NAV_LOGO_H = 33-2*3;
int TOP_NAV_LOGO_W;
int TOPNAV_MENU_BUTTON_WIDTH = 50;

TOPNAV_MENU_NODE TOPNAV_MENU[5] = {
    {
        "File",
        { "New File", "Open File", "Save", "Save As", "Exit" },
        0,
        0
    },
    {
        "Edit",
        { "Undo", "Redo", "Cut", "Copy", "Paste" },
        0,
        0
    },
    {
        "View",
        { "Zoom In", "Zoom Out", "Toggle Sidebar", "Fullscreen", "" },
        0,
        0
    },
    {
        "Run",
        { "Build", "Run", "Debug", "", "" },
        0,
        0
    },
    {
        "Help",
        { "Documentation", "Keyboard Shortcuts", "About", "", "" },
        0,
        0
    }
};

MENU_BAR_NODE LEFT_MENU[4] = {
    {
        "EXPLORER",
        "assets/filemanager_icon.png",
        "assets/filemanager_icon_active.png",
        0,
        0,
        NULL,
        NULL,
        NULL,
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        "SEARCH",
        "assets/search_icon.png",
        "assets/search_icon_active.png",
        0,
        0,
        NULL,
        NULL,
        NULL,
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        "GITHUB",
        "assets/github_icon.png",
        "assets/github_icon_active.png",
        0,
        0,
        NULL,
        NULL,
        NULL,
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        "EXTENTIONS",
        "assets/extentions_icon.png",
        "assets/extentions_icon_active.png",
        0,
        0,
        NULL,
        NULL,
        NULL,
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};



SDL_Window* window;
SDL_Renderer* renderer;

int editorState = 0;

int main(){
    SDL_Init(SDL_VIDEO_OPENGL);
    TTF_Init();
    IMG_Init(0);

    window = SDL_CreateWindow("CodeDesk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


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


    SDL_Surface* logoSurface2 = IMG_Load("assets/logo_grayscale.png");
    SDL_Texture* logoTexture2 = SDL_CreateTextureFromSurface(renderer, logoSurface2);
    SDL_FreeSurface(logoSurface2);
    int logo2AW, logo2AH;
    SDL_QueryTexture(logoTexture2, NULL, NULL, &logo2AW, &logo2AH);
    int dlw2 = 300;
    int dlh2 = (logo2AH+0.0)/logo2AW*dlw2;
    SDL_Rect logoRect2 = {MENU_BAR_W + MENU_W + (WINDOW_W - MENU_BAR_W - MENU_W)/2 - dlw2/2, TOPNAV_H + (WINDOW_H - TOPNAV_H)/2 - dlh2/2, dlw2, dlh2};





    // Initialising Fonts
    TTF_Font* font1 = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", TOP_NAV_LOGO_H/1.7);
    TTF_Font* font2 = TTF_OpenFont("assets/Montserrat/static/Montserrat-Regular.ttf", TOP_NAV_LOGO_H/1.7);

    int prevX = MENU_BAR_W + TOPNAV_PADDINGX;

    // Top Nav Left Buttons
    for (int i = 0; i < 5; i++) {
        TOPNAV_MENU_NODE* node = &TOPNAV_MENU[i];
        SDL_Color color = {155, 155, 155};

        SDL_Surface* node_surface = TTF_RenderText_Blended(font1, node->name, color);
        if (!node_surface) {
            fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
            continue;
        }

        node->texture = SDL_CreateTextureFromSurface(renderer, node_surface);
        if (!node->texture) {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(node_surface);
            continue;
        }

        SDL_Rect rect = {
            prevX,
            TOPNAV_H / 2 - node_surface->h / 2,
            node_surface->w,
            node_surface->h
        };
        node->rect = rect;

        prevX += node_surface->w + TOPNAV_PADDINGX;

        SDL_FreeSurface(node_surface);
    }

    // Menu Bar Buttons
    int prevY = TOPNAV_H + 20;
    for (int i = 0; i < sizeof(LEFT_MENU)/sizeof(LEFT_MENU[0]); i++) {
        MENU_BAR_NODE* node = &LEFT_MENU[i];
        SDL_Color color = {155, 155, 155};

        SDL_Surface* node_surface = IMG_Load(node->icon);
        SDL_Surface* node_active_surface = IMG_Load(node->active_icon);
        SDL_Surface* node_text_surface = TTF_RenderText_Blended(font1, node->name, color);
        if (!node_surface || !node_active_surface || !node_text_surface) {
            fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
            continue;
        }

        node->texture = SDL_CreateTextureFromSurface(renderer, node_surface);
        node->active_texture = SDL_CreateTextureFromSurface(renderer, node_active_surface);
        node->text_texture = SDL_CreateTextureFromSurface(renderer, node_text_surface);
        if (!node->texture || !node->active_texture || !node->text_texture) {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(node_surface);
            continue;
        }

        int viewW = MENU_BAR_W-25;
        int viewH = (viewW)*(node_surface->h+0.0)/node_surface->w;

        SDL_Rect rect = {
            MENU_BAR_W/2 - viewW/2,
            prevY,
            viewW,
            viewH
        };
        node->rect = rect;

        SDL_Rect rect2 = {
            MENU_BAR_W + MENU_PAD_X,
            TOPNAV_H + MENU_PAD_Y,
            node_text_surface->w,
            node_text_surface->h
        };

        node->text_rect = rect2;

        prevY += viewH + 20;

        SDL_FreeSurface(node_surface);
    }





    // Creating Top Nav and Left Menu
    SDL_Rect TOPNAV_bg_rect = {0, 0, WINDOW_W, TOPNAV_H};
    SDL_Rect MENUBAR_bg_rect = {0, TOPNAV_H, MENU_BAR_W, WINDOW_H};
    SDL_Rect MENU_bg_rect = {MENU_BAR_W, TOPNAV_H, MENU_W, WINDOW_H};


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

                // Menu Bar Buttons
                if(x<MENU_BAR_W+50){
                    for(i=0;i<sizeof(LEFT_MENU)/sizeof(LEFT_MENU[0]);i++){
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
                                }
                                else{
                                    showMenu = 0;
                                    logoRect2.x = MENU_BAR_W + (WINDOW_W-MENU_BAR_W)/2 - dlw2/2;
                                }
                            } else {
                                showMenu = 1;
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
                    for(i=0;i<sizeof(LEFT_MENU)/sizeof(LEFT_MENU[0]);i++){
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
                
            }
        }

        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
        SDL_RenderClear(renderer);


        /* ===== Draw Home Screen ===== */


        // ===== Top Nav =====
        SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
        SDL_RenderFillRect(renderer, &TOPNAV_bg_rect);

        SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);
        

        // Left Menu
        SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
        SDL_RenderFillRect(renderer, &MENUBAR_bg_rect);
        if(showMenu) SDL_RenderFillRect(renderer, &MENU_bg_rect);

        SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);
        SDL_RenderDrawLine(renderer, MENU_BAR_W, TOPNAV_H, MENU_BAR_W, WINDOW_H); // Drawing MENUBAR Border
        if(showMenu)
        SDL_RenderDrawLine(renderer, MENU_BAR_W + MENU_W, TOPNAV_H, MENU_BAR_W + MENU_W, WINDOW_H); // Drawing MENU Border
        SDL_RenderDrawLine(renderer, 0, TOPNAV_H, WINDOW_W, TOPNAV_H); // Drawing TOPNAV Border



        // Top Nav Left Buttons
        for(i=0;i<5;i++){
            TOPNAV_MENU_NODE node = TOPNAV_MENU[i];
            if(node.isActive){
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                SDL_Rect r;
                r.x = node.rect.x-7.9;
                r.w = node.rect.w+16.8;
                r.y = node.rect.y;
                r.h = node.rect.h;
                SDL_RenderFillRect(renderer, &r);
            }
            SDL_RenderCopy(renderer, node.texture, NULL, &node.rect);
        }

        // Menu Bar Buttons
        for(i=0;i<sizeof(LEFT_MENU)/sizeof(LEFT_MENU[0]);i++){
            MENU_BAR_NODE node = LEFT_MENU[i];
            if(menu_state == i){
                SDL_Rect r = {
                    0,
                    node.rect.y - 10,
                    3,
                    node.rect.h + 20
                };
                SDL_SetRenderDrawColor(renderer, 50, 50, 130, 255);
                SDL_RenderFillRect(renderer, &r);
            }

            if(node.isActive || menu_state==i){
                SDL_RenderCopy(renderer, node.active_texture, NULL, &node.rect);
            } else {
                SDL_RenderCopy(renderer, node.texture, NULL, &node.rect);
            }

        }

        // ===== Drawing Left Menu Components =====
        if(menu_state == 0) renderExplorer();
        if(menu_state == 1) renderSearch();
        if(menu_state == 2) renderGithub();
        if(menu_state == 3) renderExtentions();

        // Drawing Logos
        SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
        if(editorState == 0){
            SDL_RenderCopy(renderer, logoTexture2, NULL, &logoRect2);
        }


        SDL_RenderPresent(renderer);
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderExplorer(){
    MENU_BAR_NODE node = LEFT_MENU[0];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);
}

void renderSearch(){
    MENU_BAR_NODE node = LEFT_MENU[1];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);
}

void renderGithub(){
    MENU_BAR_NODE node = LEFT_MENU[2];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);
}

void renderExtentions(){
    MENU_BAR_NODE node = LEFT_MENU[3];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);
}