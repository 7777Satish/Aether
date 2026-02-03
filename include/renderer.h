#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "files.h"
#include "completion.h"

typedef struct
{
    char name[10];
    char list[10][20];
    int isActive;
    int clicked;
    SDL_Texture *texture;
    SDL_Rect rect;
} TOPNAV_MENU_NODE;

typedef struct
{
    char name[15];
    char icon[40];
    char active_icon[40];
    int isActive;
    int clicked;
    SDL_Texture *texture;
    SDL_Texture *active_texture;
    SDL_Texture *text_texture;
    SDL_Rect rect;
    SDL_Rect text_rect;
} MENU_BAR_NODE;

typedef struct
{
    SDL_Texture *t1;
    SDL_Texture *t2;
    SDL_Texture *t3;
    SDL_Texture *t4;
    SDL_Texture *t5;
    SDL_Rect r1;
    SDL_Rect r2;
    SDL_Rect r3;
    SDL_Rect r4;
    SDL_Rect r5;
} ELEMENT;

typedef struct
{
    char txt1[30];
    char txt2[30];
    char txt3[30];
    char txt4[30];
    char txt5[310];
} Search_TEXTBOXES;

typedef struct Cursor{
    int w;
    int h;
    int x;
    int y;
} Cursor;

typedef struct FileNode FileNode;

extern TOPNAV_MENU_NODE TOPNAV_MENU[5];
extern MENU_BAR_NODE TOPNAV_RIGHT[1];

extern MENU_BAR_NODE LEFT_MENU[4];

extern ELEMENT Explorer;
extern ELEMENT Search;

extern Search_TEXTBOXES SearchMenu;

extern ELEMENT Github;
extern ELEMENT Extentions;
extern ELEMENT Footer;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern int WINDOW_W;
extern int WINDOW_H;

/* ===== Mouse and Keyboard =====*/
extern int MOUSE_X;
extern int MOUSE_Y;
extern int IS_MOUSE_DOWN;
extern int IS_SELECTING;

// Initialising Left Menu
extern int MENU_BAR_H;
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

// Initialising FOOTER
extern int FOOTER_H;

// Initialising Editor
extern int EDITORMENU_H;
extern int EDITOR_PADDINGX;
extern int EDITOR_PADDINGY;
extern int EDITOR_FONT_SIZE;
extern int EDITOR_FONT_HEIGHT;
extern int MINIMAP_W;

// Fonts
extern TTF_Font *poppins_regular;
extern TTF_Font *poppins_bold;
extern TTF_Font *font2;
extern TTF_Font *jetbrains_regular;

// Creating Top Nav and Left Menu
extern SDL_Rect TOPNAV_bg_rect;
extern SDL_Rect MENUBAR_bg_rect;
extern SDL_Rect MENU_bg_rect;
extern SDL_Rect FILEBAR_bg_rect;

// Initialising Right Panel
extern int RIGHTPANEL_X;
extern int RIGHTPANEL_W;

/* ===== Scrollbars =====*/
extern int EXPLORER_SCROLL_Y;
extern int FILEMENU_SCROLL_X;

/* ===== Initialising Logos ===== */

extern SDL_Surface *logoSurface2;
extern SDL_Texture *logoTexture2;
extern int logo2AW, logo2AH;
extern int dlw2;
extern int dlh2;
extern SDL_Rect logoRect2;

extern SDL_Surface *logoSurface;
extern SDL_Texture *logoTexture;
extern int logoAW, logoAH;
extern SDL_Rect logoRect;

extern Cursor* cursor;

extern SDL_Color white;

void init();

void renderTopNav();
void renderTopNavBarMenu();
void renderMenuBar();
void renderFileBar();
void renderTextEditor();
void renderSuggestionBox();

void renderExplorer();
void renderFolder(FileNode **folder, int *i, int padX);
void renderSearch();
void renderGithub();
void renderExtentions();
void renderFooter();

#endif