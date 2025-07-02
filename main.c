#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


void renderExplorer();
void renderSearch();
void renderGithub();
void renderExtentions();

// Fonts
TTF_Font* poppins_regular;
TTF_Font* poppins_bold; 
TTF_Font* font2;

SDL_Window* window;
SDL_Renderer* renderer;

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

typedef struct{
    SDL_Texture* t1;
    SDL_Texture* t2;
    SDL_Texture* t3;
    SDL_Texture* t4;
    SDL_Texture* t5;
    SDL_Rect r1;
    SDL_Rect r2;
    SDL_Rect r3;
    SDL_Rect r4;
    SDL_Rect r5;
} ELEMENT;

typedef struct {
    char txt1[30];
    char txt2[30];
    char txt3[30];
    char txt4[30];
    char txt5[310];
} Search_TEXTBOXES;

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

ELEMENT Explorer = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};
ELEMENT Search = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};

Search_TEXTBOXES SearchMenu = {
    "Search",
    "Replace",
    "",
    "",
    ""
};

ELEMENT Github = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};
ELEMENT Extentions = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};

// File Management
char filesrc[50] = "/home/greatme/Desktop/sample/text.txt";
char *fileContent;

char* readFile(const char* filesrc) {
    FILE *f = fopen(filesrc, "r");
    if (!f) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* fileContent = malloc(size + 1);  // +1 for '\0'
    if (!fileContent) {
        fclose(f);
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    fread(fileContent, 1, size, f);
    fileContent[size] = '\0'; // Null-terminate the string

    fclose(f);
    return fileContent;
}

// ======== LEFT MENU EXPLORER ========

int editorState = 0;

int main(){

    fileContent = readFile(filesrc);
    if (fileContent) {
        printf("%s", fileContent);
        free(fileContent);
    }

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





    /* ===== Initialising Fonts ===== */
    poppins_regular = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", TOP_NAV_LOGO_H/1.7);
    poppins_bold = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", TOP_NAV_LOGO_H/1.7);
    font2 = TTF_OpenFont("assets/Montserrat/static/Montserrat-Regular.ttf", TOP_NAV_LOGO_H/1.7 - 1);

    int prevX = MENU_BAR_W + TOPNAV_PADDINGX;

    
    // Top Nav Left Buttons
    for (int i = 0; i < 5; i++) {
        TOPNAV_MENU_NODE* node = &TOPNAV_MENU[i];
        SDL_Color color = {155, 155, 155};

        SDL_Surface* node_surface = TTF_RenderText_Blended(poppins_regular, node->name, color);
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
        SDL_Surface* node_text_surface = TTF_RenderText_Blended(poppins_regular, node->name, color);
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
                // SDL_SetRenderDrawColor(renderer, 50, 50, 130, 255);
                
                SDL_SetRenderDrawColor(renderer, 106, 90, 205, 255);
                SDL_RenderFillRect(renderer, &r);
            }

            if(node.isActive || menu_state==i){
                SDL_RenderCopy(renderer, node.active_texture, NULL, &node.rect);
            } else {
                SDL_RenderCopy(renderer, node.texture, NULL, &node.rect);
            }

        }

        // ===== Drawing Left Menu Components =====
        if(showMenu){
            if(menu_state == 0) renderExplorer();
            if(menu_state == 1) renderSearch();
            if(menu_state == 2) renderGithub();
            if(menu_state == 3) renderExtentions();
        }

        // Drawing Logos
        SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
        if(editorState == 0){
            SDL_RenderCopy(renderer, logoTexture2, NULL, &logoRect2);
        }


        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }
    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderExplorer(){
    MENU_BAR_NODE node = LEFT_MENU[0];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {180, 180, 180};

    // Textures
    if(!Explorer.t1){
        SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, "You have not opened", color);
        Explorer.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    if(!Explorer.t2){
        SDL_Surface* s2 = TTF_RenderText_Blended(poppins_regular, "any folder yet", color);
        Explorer.t2 = SDL_CreateTextureFromSurface(renderer, s2);
    }


    if(!Explorer.t3){
        SDL_Color color = {233, 233, 233};
        SDL_Surface* s3 = TTF_RenderText_Blended(font2, "Open Folder", color);
        Explorer.t3 = SDL_CreateTextureFromSurface(renderer, s3);
    }

    // if(!Explorer.t4){
    //     SDL_Surface* s4 = TTF_RenderText_Blended(poppins_regular, "Open Recent", color);
    //     Explorer.t4 = SDL_CreateTextureFromSurface(renderer, s4);
    // }

    // Rects
    if(!Explorer.r1.x){
        int w, h;
        SDL_QueryTexture(Explorer.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h
        };
        Explorer.r1 = r1; // First Text Box
    }

    if(!Explorer.r2.x){
        int w, h;
        SDL_QueryTexture(Explorer.t2, NULL, NULL, &w, &h);
        SDL_Rect r2 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + Explorer.r1.h,
            w,
            h
        };
        Explorer.r2 = r2; // Second Text Box
    }

    if(!Explorer.r3.x){
        int w, h;
        SDL_QueryTexture(Explorer.t1, NULL, NULL, &w, &h);
        SDL_Rect r3 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3*MENU_PAD_Y + Explorer.r1.h + Explorer.r2.h - 2.5,
            MENU_W - 2 * MENU_PAD_Y,
            h + 5
        };
        Explorer.r3 = r3; // First Button Container
    }

    if(!Explorer.r4.x){
        int w, h;
        SDL_QueryTexture(Explorer.t3, NULL, NULL, &w, &h);
        SDL_Rect r4 = {
            MENU_BAR_W + MENU_W/2 - w/2,
            node.text_rect.y + node.text_rect.h + 3*MENU_PAD_Y + Explorer.r1.h + Explorer.r2.h,
            w,
            h
        };
        Explorer.r4 = r4; // First Button Text Container
    }

    SDL_SetRenderDrawColor(renderer, 106, 90, 205, 255);
    SDL_RenderFillRect(renderer, &Explorer.r3);
    SDL_RenderCopy(renderer, Explorer.t1, NULL, &Explorer.r1);
    SDL_RenderCopy(renderer, Explorer.t2, NULL, &Explorer.r2);
    SDL_RenderCopy(renderer, Explorer.t3, NULL, &Explorer.r4);

}

void renderSearch(){
    MENU_BAR_NODE node = LEFT_MENU[1];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {100, 100, 100};

    // Textures
    if(!Search.t1){
        SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, SearchMenu.txt1, color);
        Search.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    if(!Search.t2){
        SDL_Surface* s2 = TTF_RenderText_Blended(poppins_regular, SearchMenu.txt2, color);
        Search.t2 = SDL_CreateTextureFromSurface(renderer, s2);
    }

    // Rects

    if(!Search.r1.x){
        int w,h;
        SDL_QueryTexture(Search.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            MENU_W - 2 * MENU_PAD_X,
            h + 5
        };
        Search.r1 = r1;
    }
    
    if(!Search.r2.x){
        int w,h;
        SDL_QueryTexture(Search.t1, NULL, NULL, &w, &h);
        SDL_Rect r2 = {
            MENU_BAR_W + MENU_PAD_X + 10,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + 2.5,
            w,
            h
        };
        Search.r2 = r2;
    }

    if(!Search.r3.x){
        int w,h;
        SDL_QueryTexture(Search.t2, NULL, NULL, &w, &h);
        SDL_Rect r3 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3.6/2 * MENU_PAD_Y + Search.r1.h,
            MENU_W - 2 * MENU_PAD_X,
            h + 5
        };
        Search.r3 = r3;
    }
    
    if(!Search.r4.x){
        int w,h;
        SDL_QueryTexture(Search.t2, NULL, NULL, &w, &h);
        SDL_Rect r4 = {
            MENU_BAR_W + MENU_PAD_X + 10,
            node.text_rect.y + node.text_rect.h + 3.6/2 * MENU_PAD_Y + 2.5 + Search.r1.h,
            w,
            h
        };
        Search.r4 = r4;
    }

    SDL_SetRenderDrawColor(renderer, 6, 6, 6, 255);
    SDL_RenderFillRect(renderer, &Search.r1);
    SDL_RenderFillRect(renderer, &Search.r3);
    SDL_RenderCopy(renderer, Search.t1, NULL, &Search.r2);
    SDL_RenderCopy(renderer, Search.t2, NULL, &Search.r4);
}

void renderGithub(){
    MENU_BAR_NODE node = LEFT_MENU[2];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {180, 180, 180};

    // Textures
    if(!Github.t1){
        SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, "Import From a GitHub", color);
        Github.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    if(!Github.t2){
        SDL_Surface* s2 = TTF_RenderText_Blended(poppins_regular, "Repository", color);
        Github.t2 = SDL_CreateTextureFromSurface(renderer, s2);
    }


    if(!Github.t3){
        SDL_Color color = {233, 233, 233};
        SDL_Surface* s3 = TTF_RenderText_Blended(font2, "Cennect Github", color);
        Github.t3 = SDL_CreateTextureFromSurface(renderer, s3);
    }

    // if(!Github.t4){
    //     SDL_Surface* s4 = TTF_RenderText_Blended(poppins_regular, "Open Recent", color);
    //     Github.t4 = SDL_CreateTextureFromSurface(renderer, s4);
    // }

    // Rects
    if(!Github.r1.x){
        int w, h;
        SDL_QueryTexture(Github.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h
        };
        Github.r1 = r1; // First Text Box
    }

    if(!Github.r2.x){
        int w, h;
        SDL_QueryTexture(Github.t2, NULL, NULL, &w, &h);
        SDL_Rect r2 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + Github.r1.h,
            w,
            h
        };
        Github.r2 = r2; // Second Text Box
    }

    if(!Github.r3.x){
        int w, h;
        SDL_QueryTexture(Github.t1, NULL, NULL, &w, &h);
        SDL_Rect r3 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3*MENU_PAD_Y + Github.r1.h + Github.r2.h - 2.5,
            MENU_W - 2 * MENU_PAD_Y,
            h + 5
        };
        Github.r3 = r3; // First Button Container
    }

    if(!Github.r4.x){
        int w, h;
        SDL_QueryTexture(Github.t3, NULL, NULL, &w, &h);
        SDL_Rect r4 = {
            MENU_BAR_W + MENU_W/2 - w/2,
            node.text_rect.y + node.text_rect.h + 3*MENU_PAD_Y + Github.r1.h + Github.r2.h,
            w,
            h
        };
        Github.r4 = r4; // First Button Text Container
    }

    SDL_SetRenderDrawColor(renderer, 106, 90, 205, 255);
    SDL_RenderFillRect(renderer, &Github.r3);
    SDL_RenderCopy(renderer, Github.t1, NULL, &Github.r1);
    SDL_RenderCopy(renderer, Github.t2, NULL, &Github.r2);
    SDL_RenderCopy(renderer, Github.t3, NULL, &Github.r4);

}

void renderExtentions(){
    MENU_BAR_NODE node = LEFT_MENU[3];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {130, 130, 130};

    // Textures
    if(!Extentions.t1){
        SDL_Surface* s1 = TTF_RenderText_Blended(poppins_regular, "Feature Coming Soon", color);
        Extentions.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    // Rects
    if(!Extentions.r1.x){
        int w, h;
        SDL_QueryTexture(Extentions.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_BAR_W + MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h
        };
        Extentions.r1 = r1; // First Text Box
    }


    SDL_RenderCopy(renderer, Extentions.t1, NULL, &Extentions.r1);
}