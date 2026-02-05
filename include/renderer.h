#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "files.h"
#include "completion.h"

// // SDL3 Compatibility wrappers
// static inline int SDL_RenderFillRect_Compat(SDL_Renderer *renderer, const SDL_FRect *rect) {
//     if (rect) {
//         SDL_FRect frect = {(float)rect->x, (float)rect->y, (float)rect->w, (float)rect->h};
//         return SDL_RenderFillRect(renderer, &frect);
//     } else {
//         return SDL_RenderFillRect(renderer, NULL);
        
//     }
// }
// #define SDL_RenderFillRect SDL_RenderFillRect_Compat

// static inline int SDL_RenderDrawRect_Compat(SDL_Renderer *renderer, const SDL_FRect *rect) {
//     if (rect) {
//         SDL_FRect frect = {(float)rect->x, (float)rect->y, (float)rect->w, (float)rect->h};
//         return SDL_RenderRect(renderer, &frect);
//     } else {
//         return SDL_RenderRect(renderer, NULL);
//     }
// }
// #define SDL_RenderDrawRect SDL_RenderDrawRect_Compat

// static inline int SDL_RenderDrawLine_Compat(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
//     return SDL_RenderLine(renderer, (float)x1, (float)y1, (float)x2, (float)y2);
// }
// #define SDL_RenderDrawLine SDL_RenderDrawLine_Compat

// static inline int SDL_RenderCopy_Compat(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, const SDL_FRect *dstrect) {
//     SDL_FRect fsrc;
//     SDL_FRect fdst;
//     SDL_FRect *psrc = NULL;
//     SDL_FRect *pdst = NULL;

//     if (srcrect) {
//         fsrc.x = (float)srcrect->x; fsrc.y = (float)srcrect->y; fsrc.w = (float)srcrect->w; fsrc.h = (float)srcrect->h;
//         psrc = &fsrc;
//     }
//     if (dstrect) {
//         fdst.x = (float)dstrect->x; fdst.y = (float)dstrect->y; fdst.w = (float)dstrect->w; fdst.h = (float)dstrect->h;
//         pdst = &fdst;
//     }
//     return SDL_RenderTexture(renderer, texture, psrc, pdst);
// }
// #define SDL_RenderCopy SDL_RenderCopy_Compat

// static inline int SDL_RenderCopyEx_Compat(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, const SDL_FRect *dstrect, double angle, const SDL_FPoint *center, SDL_FlipMode flip) {
//     SDL_FRect fsrc;
//     SDL_FRect fdst;
//     SDL_FRect *psrc = NULL;
//     SDL_FRect *pdst = NULL;
//     SDL_FPoint fcenter;
//     SDL_FPoint *pcenter = NULL;

//     if (srcrect) {
//         fsrc.x = (float)srcrect->x; fsrc.y = (float)srcrect->y; fsrc.w = (float)srcrect->w; fsrc.h = (float)srcrect->h;
//         psrc = &fsrc;
//     }
//     if (dstrect) {
//         fdst.x = (float)dstrect->x; fdst.y = (float)dstrect->y; fdst.w = (float)dstrect->w; fdst.h = (float)dstrect->h;
//         pdst = &fdst;
//     }
//     if (center) {
//         fcenter.x = (float)center->x; fcenter.y = (float)center->y;
//         pcenter = &fcenter;
//     }
//     return SDL_RenderTextureRotated(renderer, texture, psrc, pdst, angle, pcenter, flip);
// }
// #define SDL_RenderCopyEx SDL_RenderCopyEx_Compat

// static inline SDL_Surface* TTF_RenderText_Blended_Compat(TTF_Font *font, const char *text, SDL_Color fg) {
//     if (text == NULL) return NULL;
//     return TTF_RenderText_Blended(font, text, 0, fg);
// }
// #define TTF_RenderText_Blended TTF_RenderText_Blended_Compat

typedef struct
{
    char name[10];
    char list[10][20];
    int isActive;
    int clicked;
    SDL_Texture *texture;
    SDL_FRect rect;
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
    SDL_FRect rect;
    SDL_FRect text_rect;
} MENU_BAR_NODE;

typedef struct
{
    SDL_Texture *t1;
    SDL_Texture *t2;
    SDL_Texture *t3;
    SDL_Texture *t4;
    SDL_Texture *t5;
    SDL_FRect r1;
    SDL_FRect r2;
    SDL_FRect r3;
    SDL_FRect r4;
    SDL_FRect r5;
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

extern int WINDOW_FONT_SIZE;

// Fonts
extern TTF_Font *poppins_regular;
extern TTF_Font *poppins_bold;
extern TTF_Font *font2;
extern TTF_Font *jetbrains_regular;

// Creating Top Nav and Left Menu
extern SDL_FRect TOPNAV_bg_rect;
extern SDL_FRect MENUBAR_bg_rect;
extern SDL_FRect MENU_bg_rect;
extern SDL_FRect FILEBAR_bg_rect;

// Initialising Right Panel
extern int RIGHTPANEL_X;
extern int RIGHTPANEL_W;

/* ===== Scrollbars =====*/
extern int EXPLORER_SCROLL_Y;
extern int FILEMENU_SCROLL_X;

/* ===== Initialising Logos ===== */

extern SDL_Surface *logoSurface2;
extern SDL_Texture *logoTexture2;
extern float logo2AW, logo2AH;
extern int dlw2;
extern int dlh2;
extern SDL_FRect logoRect2;

extern SDL_Surface *logoSurface;
extern SDL_Texture *logoTexture;
extern int logoAW, logoAH;
extern SDL_FRect logoRect;

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
void renderRightPanel();

#endif