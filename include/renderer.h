#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "files.h"


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


extern TOPNAV_MENU_NODE TOPNAV_MENU[5];

extern MENU_BAR_NODE LEFT_MENU[4];

extern ELEMENT Explorer;
extern ELEMENT Search;

extern Search_TEXTBOXES SearchMenu;

extern ELEMENT Github;
extern ELEMENT Extentions;



extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern int WINDOW_W;
extern int WINDOW_H;

// Initialising Left Menu
extern int MENU_BAR_W;
extern int MENU_W;
extern int MENU_PAD_X;
extern int MENU_PAD_Y;
extern int showMenu;
extern int menu_state;

// Initialising TOPNAV
extern int TOPNAV_H;
extern int TOPNAV_PADDINGX;
extern int TOPNAV_PADDINGY;
extern int TOP_NAV_LOGO_H;
extern int TOP_NAV_LOGO_W;
extern int TOPNAV_MENU_BUTTON_WIDTH;
extern int IS_TOPNAV_MENU_DOWN;

// Initialising Editor
extern int EDITORMENU_H;
extern int EDITOR_PADDINGX;
extern int EDITOR_PADDINGY;

// Fonts
extern TTF_Font* poppins_regular;
extern TTF_Font* poppins_bold; 
extern TTF_Font* font2;

// Creating Top Nav and Left Menu
extern SDL_Rect TOPNAV_bg_rect;
extern SDL_Rect MENUBAR_bg_rect;
extern SDL_Rect MENU_bg_rect;

void init();

void renderTopNav();
void renderTopNavBarMenu();
void renderMenuBar();

void renderExplorer();
void renderFolder(FileNode** folder, int* i, int padX);
void renderSearch();
void renderGithub();
void renderExtentions();

#endif
