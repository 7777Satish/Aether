/**
 * @file renderer.c
 * @brief SDL2-based rendering system for the code editor
 *
 * This module handles all graphics rendering operations including:
 * - SDL2 initialization and window management
 * - UI component rendering (menus, panels, text)
 * - Font management and text rendering
 * - Theme and color management
 * - Layout calculations and positioning
 */

#include "renderer.h"

// ======== UI MENU DEFINITIONS ========

/**
 * @brief Top navigation menu configuration
 *
 * Defines the main menu bar items (File, Edit, View, Run, Help)
 * with their respective dropdown menu items and initial state.
 */
TOPNAV_MENU_NODE TOPNAV_MENU[5] = {
    {"File",
     {"New File", "Open File", "Save", "Save As", "Exit"},
     0,             // active
     0,             // clicked
     NULL,          // surface
     {0, 0, 0, 0}}, // rect
    {"Edit",
     {"Undo", "Redo", "Cut", "Copy", "Paste"},
     0,
     0,
     NULL,
     {0, 0, 0, 0}},
    {"View",
     {"Zoom In", "Zoom Out", "Toggle Sidebar", "Fullscreen", ""},
     0,
     0,
     NULL,
     {0, 0, 0, 0}},
    {"Run",
     {"Build", "Run", "Debug", "", ""},
     0,
     0,
     NULL,
     {0, 0, 0, 0}},
    {"Help",
     {"Documentation", "Keyboard Shortcuts", "About", "", ""},
     0,
     0,
     NULL,
     {0, 0, 0, 0}}};

MENU_BAR_NODE TOPNAV_RIGHT[1] = {
    {"RIGHT PANEL",
     "assets/panel-right.png",
     "assets/panel-right-active.png",
     0,
     0,
     NULL,
     NULL,
     NULL,
     {0, 0, 0, 0},
     {0, 0, 0, 0}}};

MENU_BAR_NODE LEFT_MENU[4] = {
    {"EXPLORER",
     "assets/filemanager_icon.png",
     "assets/filemanager_icon_active.png",
     0,
     0,
     NULL,
     NULL,
     NULL,
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {"SEARCH",
     "assets/search_icon.png",
     "assets/search_icon_active.png",
     0,
     0,
     NULL,
     NULL,
     NULL,
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {"GITHUB",
     "assets/github_icon.png",
     "assets/github_icon_active.png",
     0,
     0,
     NULL,
     NULL,
     NULL,
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {"EXTENTIONS",
     "assets/extentions_icon.png",
     "assets/extentions_icon_active.png",
     0,
     0,
     NULL,
     NULL,
     NULL,
     {0, 0, 0, 0},
     {0, 0, 0, 0}}};

ELEMENT Explorer = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};
ELEMENT Search = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};

Search_TEXTBOXES SearchMenu = {
    "Search",
    "Replace",
    "",
    "",
    ""};

ELEMENT Github = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};
ELEMENT Extentions = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};
ELEMENT Footer = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};

ELEMENT FileIcons = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};
ELEMENT FileIcons2 = {NULL, NULL, NULL, NULL, NULL, {}, {}, {}, {}, {}};

SDL_Window *window = NULL;

SDL_Renderer *renderer = NULL;

int WINDOW_W = 1400;
int WINDOW_H = 800;

/* ===== Mouse and Keyboard =====*/
int MOUSE_X = 0;
int MOUSE_Y = 0;
int IS_MOUSE_DOWN = 0;
int IS_SELECTING = 0;

// Initialising Left Menu
int MENU_BAR_H = 50;
int MENU_W = 250;
int MENU_PAD_X = 10;
int MENU_PAD_Y = 10;
int showMenu = 1;
int menu_state = 0;

// Initialising TOPNAV
// int TOPNAV_H = 33;
int TOPNAV_H = 40;
int TOPNAV_PADDINGX = 20;
int TOPNAV_PADDINGY = 6;
int TOP_NAV_LOGO_H = 33 - 2 * 3;
int TOP_NAV_LOGO_W;
int TOPNAV_MENU_BUTTON_WIDTH = 50;
int IS_TOPNAV_MENU_DOWN = 0;

// Initialising Footer
int FOOTER_H = 33;

// Initialising Editor
int EDITORMENU_H = 30;
int EDITOR_PADDINGX = 10;
int EDITOR_PADDINGY = 10;
int EDITOR_FONT_SIZE = 0;
int EDITOR_FONT_HEIGHT = 0;
int MINIMAP_W = 100;
Cursor *cursor;

int WINDOW_FONT_SIZE = 0;

// Initialising Right Panel
int RIGHTPANEL_W = 400;
int RIGHTPANEL_X = 1400;

SDL_Color white = {255, 255, 255, 255};

// Fonts
TTF_Font *poppins_regular = NULL;
TTF_Font *poppins_bold = NULL;
TTF_Font *font2 = NULL;
TTF_Font *jetbrains_regular = NULL;

SDL_Texture *DIGIT_TEXTURES[10] = {NULL};
int DIGIT_W = 0;
int DIGIT_H = 0;

// Creating Top Nav and Left Menu
SDL_FRect TOPNAV_bg_rect = {};
SDL_FRect MENUBAR_bg_rect = {};
SDL_FRect MENU_bg_rect = {};
SDL_FRect FILEBAR_bg_rect = {};

/* ===== Initialising Logos ===== */

SDL_Surface *logoSurface2 = NULL;
SDL_Texture *logoTexture2 = NULL;
float logo2AW, logo2AH;
int dlw2 = 300;
int dlh2;
SDL_FRect logoRect2;

SDL_Surface *logoSurface = NULL;
SDL_Texture *logoTexture = NULL;
int logoAW, logoAH;
SDL_FRect logoRect = {};

/* ===== Scrollbars =====*/
int EXPLORER_SCROLL_Y = 0;
int FILEMENU_SCROLL_X = 0;

/**
 * @brief Initializes the entire application rendering system
 *
 * This function sets up SDL2, creates the main window and renderer,
 * loads all fonts and assets, and configures the initial UI layout.
 * Must be called once at application startup before any rendering.
 *
 * @note Will exit the program if critical resources fail to load
 */
void init()
{
    // ======== SDL2 INITIALIZATION ========
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    /* Create window FIRST */
    window = SDL_CreateWindow(
        "Aether",
        WINDOW_W,
        WINDOW_H,
        SDL_WINDOW_RESIZABLE);

    /* Create renderer SECOND */
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        printf("Renderer error: %s\n", SDL_GetError());
        return;
    }
    
    /* Now configure renderer */
    SDL_SetRenderVSync(renderer, 1); // keep this OFF for editor
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // SDL_SetRenderLogicalPresentation(
    //     renderer,
    //     WINDOW_W,
    //     WINDOW_H,
    //     SDL_LOGICAL_PRESENTATION_DISABLED);

    SDL_StartTextInput(window);

    EDITOR_FONT_SIZE = TOP_NAV_LOGO_H / 1.5;
    // EDITOR_FONT_SIZE = TOP_NAV_LOGO_H / 1.5;
    WINDOW_FONT_SIZE = TOP_NAV_LOGO_H / 1.7;

    // printf("Renderer: %s\n", SDL_GetRendererName(renderer));

    poppins_regular = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", WINDOW_FONT_SIZE);
    if (!poppins_regular)
    {
        fprintf(stderr, "Failed to load Poppins-Regular.ttf: %s\n", SDL_GetError());
        exit(1);
    }
    
    // Initialize completion textures with the loaded font and current renderer
    initCompletion(renderer, poppins_regular);

    poppins_bold = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", WINDOW_FONT_SIZE);
    if (!poppins_bold)
    {
        fprintf(stderr, "Failed to load Poppins-Bold.ttf: %s\n", SDL_GetError());
        exit(1);
    }

    font2 = TTF_OpenFont("assets/Montserrat/static/Montserrat-Regular.ttf", WINDOW_FONT_SIZE - 1);
    if (!font2)
    {
        fprintf(stderr, "Failed to load Montserrat-Regular.ttf: %s\n", SDL_GetError());
        exit(1);
    }

    jetbrains_regular = TTF_OpenFont("assets/JetBrains_Mono/static/JetBrainsMono-Regular.ttf", EDITOR_FONT_SIZE);
    if (!jetbrains_regular)
    {
        fprintf(stderr, "Failed to load JetBrainsMono-Regular.ttf: %s\n", SDL_GetError());
        exit(1);
    }

    /* ===== Digit Cache Initialization ===== */
    for (int i = 0; i < 10; i++)
    {
        char digit[2];
        sprintf(digit, "%d", i);
        SDL_Surface *surface = TTF_RenderText_Blended(jetbrains_regular, digit, 1, (SDL_Color){155, 155, 155, 255});
        if (surface) {
            DIGIT_TEXTURES[i] = SDL_CreateTextureFromSurface(renderer, surface);
            if (i == 0) {
                DIGIT_W = surface->w;
                DIGIT_H = surface->h;
            }
            SDL_DestroySurface(surface);
        }
    }

    /* ===== Initialising Cursor ===== */

    SDL_Surface *space = TTF_RenderText_Blended(jetbrains_regular, " ", 1, (SDL_Color){0, 0, 0, 255});

    cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->w = space->w;
    cursor->h = space->h;
    cursor->x = -1;
    cursor->y = -1;
    EDITOR_FONT_HEIGHT = space->h;
    SDL_DestroySurface(space);

    /* ===== Initialising Logos ===== */

    logoSurface2 = IMG_Load("assets/logo_grayscale.png");
    logoTexture2 = SDL_CreateTextureFromSurface(renderer, logoSurface2);
    // SDL_SetTextureScaleMode(logoTexture2, SDL_SCALEMODE_NEAREST);

    SDL_DestroySurface(logoSurface2);
    SDL_GetTextureSize(logoTexture2, &logo2AW, &logo2AH);

    dlw2 = 300;
    dlh2 = (logo2AH + 0.0) / logo2AW * dlw2;

    logoRect2.x = MENU_W + (WINDOW_W - MENU_W) / 2 - dlw2 / 2;
    logoRect2.y = TOPNAV_H + (WINDOW_H - TOPNAV_H) / 2 - dlh2 / 2;
    logoRect2.w = dlw2;
    logoRect2.h = dlh2;

    /* NavBar Logo Start */
    logoSurface = IMG_Load("assets/logo.png");
    logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    // SDL_SetTextureScaleMode(logoTexture, SDL_SCALEMODE_NEAREST);

    int logoAW = logoSurface->w, logoAH = logoSurface->h;
    SDL_DestroySurface(logoSurface);
    TOP_NAV_LOGO_W = (logoAW + 0.0) / logoAH * TOP_NAV_LOGO_H;
    int dlh = TOPNAV_H - 2 * 10;
    int dlw = (logoAW + 0.0) / logoAH * dlh;

    logoRect.x = MENU_BAR_H / 2 - dlw / 2;
    logoRect.y = TOPNAV_H / 2 - dlh / 2;
    logoRect.w = dlw;
    logoRect.h = dlh;

    int prevX = logoRect.x + logoRect.w + TOPNAV_PADDINGX;

    // Top Nav Left Buttons
    for (int i = 0; i < 5; i++)
    {
        TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];
        SDL_Color color = {155, 155, 155, 255};

        SDL_Surface *node_surface = TTF_RenderText_Blended(poppins_regular, node->name, strlen(node->name), color);
        if (!node_surface)
        {
            fprintf(stderr, "Failed to render text: %s\n", SDL_GetError());
            continue;
        }

        node->texture = SDL_CreateTextureFromSurface(renderer, node_surface);
        // SDL_SetTextureScaleMode(node->texture, SDL_SCALEMODE_NEAREST);

        // SDL_SetTextureScaleMode(node->texture, SDL_SCALEMODE_NEAREST);

        if (!node->texture)
        {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_DestroySurface(node_surface);
            continue;
        }

        SDL_FRect rect = {
            prevX,
            TOPNAV_H / 2 - node_surface->h / 2,
            node_surface->w,
            node_surface->h};
        node->rect = rect;

        prevX += node_surface->w + TOPNAV_PADDINGX;

        SDL_DestroySurface(node_surface);
    }

    // TOPNAV_RIGHT Buttons
    prevX = 0;
    for (int i = 0; i < (int)(sizeof(TOPNAV_RIGHT) / sizeof(TOPNAV_RIGHT[0])); i++)
    {
        MENU_BAR_NODE *node = &TOPNAV_RIGHT[i];
        node->clicked = 0;
        node->isActive = 0;
        SDL_Color color = {155, 155, 155, 255};

        SDL_Surface *node_surface = IMG_Load(node->icon);
        SDL_Surface *node_active_surface = IMG_Load(node->active_icon);
        SDL_Surface *node_text_surface = TTF_RenderText_Blended(poppins_regular, node->name, strlen(node->name), color);
        if (!node_surface || !node_active_surface || !node_text_surface)
        {
            fprintf(stderr, "Failed to render text: %s\n", SDL_GetError());
            continue;
        }

        node->texture = SDL_CreateTextureFromSurface(renderer, node_surface);
        // SDL_SetTextureScaleMode(node->texture, SDL_SCALEMODE_NEAREST);

        node->active_texture = SDL_CreateTextureFromSurface(renderer, node_active_surface);
        // SDL_SetTextureScaleMode(node->active_texture, SDL_SCALEMODE_NEAREST);

        node->text_texture = SDL_CreateTextureFromSurface(renderer, node_text_surface);
        // SDL_SetTextureScaleMode(node->text_texture, SDL_SCALEMODE_NEAREST);

        if (!node->texture || !node->active_texture || !node->text_texture)
        {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_DestroySurface(node_surface);
            continue;
        }

        int viewW = MENU_BAR_H - 30;
        int viewH = (viewW) * (node_surface->h + 0.0) / node_surface->w;

        SDL_FRect rect = {
            WINDOW_W - viewW - TOPNAV_H / 2 - prevX,
            TOPNAV_H / 2 - viewH / 2,
            viewW,
            viewH};
        node->rect = rect;

        SDL_FRect rect2 = {
            MENU_PAD_X,
            TOPNAV_H + MENU_BAR_H + MENU_PAD_Y,
            node_text_surface->w,
            node_text_surface->h};

        node->text_rect = rect2;

        prevX += viewW + 20;

        SDL_DestroySurface(node_surface);
    }

    RIGHTPANEL_X = WINDOW_W;

    // Menu Bar Buttons
    prevX = 0;
    for (int i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
    {
        MENU_BAR_NODE *node = &LEFT_MENU[i];
        SDL_Color color = {155, 155, 155, 255};

        SDL_Surface *node_surface = IMG_Load(node->icon);
        SDL_Surface *node_active_surface = IMG_Load(node->active_icon);
        SDL_Surface *node_text_surface = TTF_RenderText_Blended(poppins_regular, node->name, strlen(node->name), color);
        if (!node_surface || !node_active_surface || !node_text_surface)
        {
            fprintf(stderr, "Failed to render text: %s\n", SDL_GetError());
            continue;
        }

        node->texture = SDL_CreateTextureFromSurface(renderer, node_surface);
        // SDL_SetTextureScaleMode(node->texture, SDL_SCALEMODE_NEAREST);

        node->active_texture = SDL_CreateTextureFromSurface(renderer, node_active_surface);
        // SDL_SetTextureScaleMode(node->active_texture, SDL_SCALEMODE_NEAREST);

        node->text_texture = SDL_CreateTextureFromSurface(renderer, node_text_surface);
        // SDL_SetTextureScaleMode(node->text_texture, SDL_SCALEMODE_NEAREST);

        if (!node->texture || !node->active_texture || !node->text_texture)
        {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_DestroySurface(node_surface);
            continue;
        }

        int viewW = MENU_BAR_H - 30;
        int viewH = (viewW) * (node_surface->h + 0.0) / node_surface->w;

        SDL_FRect rect = {
            MENU_BAR_H / 2 - viewW / 2 + prevX,
            TOPNAV_H + viewH,
            viewW,
            viewH};
        node->rect = rect;

        SDL_FRect rect2 = {
            MENU_PAD_X,
            TOPNAV_H + MENU_BAR_H + MENU_PAD_Y,
            node_text_surface->w,
            node_text_surface->h};

        node->text_rect = rect2;

        prevX += viewW + 20;

        SDL_DestroySurface(node_surface);
    }

    // Initializing FileIcons
    SDL_Surface *fi_s1 = IMG_Load("assets/file_icon2.png");
    FileIcons.t1 = SDL_CreateTextureFromSurface(renderer, fi_s1);
    // SDL_SetTextureScaleMode(FileIcons.t1, SDL_SCALEMODE_NEAREST);

    FileIcons.r1.w = fi_s1->w;
    FileIcons.r1.h = fi_s1->h;
    SDL_DestroySurface(fi_s1);

    SDL_Surface *fi_s2 = IMG_Load("assets/chevron_right.png");
    FileIcons.t2 = SDL_CreateTextureFromSurface(renderer, fi_s2);
    // SDL_SetTextureScaleMode(FileIcons.t2, SDL_SCALEMODE_NEAREST);
    FileIcons.r2.w = fi_s2->w;
    FileIcons.r2.h = fi_s2->h;
    SDL_DestroySurface(fi_s2);

    SDL_Surface *fi_s3 = IMG_Load("assets/chevron_down.png");
    FileIcons.t3 = SDL_CreateTextureFromSurface(renderer, fi_s3);
    // SDL_SetTextureScaleMode(FileIcons.t3, SDL_SCALEMODE_NEAREST);
    FileIcons.r3.w = fi_s3->w;
    FileIcons.r3.h = fi_s3->h;
    SDL_DestroySurface(fi_s3);

    SDL_Surface *fi_s4 = IMG_Load("assets/file_icons/icon_clang.png");
    FileIcons.t4 = SDL_CreateTextureFromSurface(renderer, fi_s4);
    // SDL_SetTextureScaleMode(FileIcons.t4, SDL_SCALEMODE_NEAREST);
    FileIcons.r4.w = fi_s4->w;
    FileIcons.r4.h = fi_s4->h;
    SDL_DestroySurface(fi_s4);

    SDL_Surface *fi_s5 = IMG_Load("assets/file_icons/icon_html.png");
    FileIcons.t5 = SDL_CreateTextureFromSurface(renderer, fi_s5);
    // SDL_SetTextureScaleMode(FileIcons.t5, SDL_SCALEMODE_NEAREST);
    FileIcons.r5.w = fi_s5->w;
    FileIcons.r5.h = fi_s5->h;
    SDL_DestroySurface(fi_s5);

    SDL_Surface *fi2_s1 = IMG_Load("assets/file_icons/icon_css.png");
    FileIcons2.t1 = SDL_CreateTextureFromSurface(renderer, fi2_s1);
    // SDL_SetTextureScaleMode(FileIcons2.t1, SDL_SCALEMODE_NEAREST);
    FileIcons2.r1.w = fi2_s1->w;
    FileIcons2.r1.h = fi2_s1->h;
    SDL_DestroySurface(fi2_s1);

    SDL_Surface *fi2_s2 = IMG_Load("assets/file_icons/icon_img.png");
    FileIcons2.t2 = SDL_CreateTextureFromSurface(renderer, fi2_s2);
    // SDL_SetTextureScaleMode(FileIcons2.t2, SDL_SCALEMODE_NEAREST);
    FileIcons2.r2.w = fi2_s2->w;
    FileIcons2.r2.h = fi2_s2->h;
    SDL_DestroySurface(fi2_s2);

    // Creating Top Nav and Left Menu
    TOPNAV_bg_rect.x = 0;
    TOPNAV_bg_rect.y = 0;
    TOPNAV_bg_rect.w = WINDOW_W;
    TOPNAV_bg_rect.h = TOPNAV_H;

    MENUBAR_bg_rect.x = 0;
    MENUBAR_bg_rect.y = TOPNAV_H;
    MENUBAR_bg_rect.w = 0;
    MENUBAR_bg_rect.h = WINDOW_H;

    MENU_bg_rect.x = 0;
    MENU_bg_rect.y = TOPNAV_H;
    MENU_bg_rect.w = MENU_W;
    MENU_bg_rect.h = WINDOW_H;

    if (showMenu)
        FILEBAR_bg_rect.x = MENU_W;
    else
        FILEBAR_bg_rect.x = 0;
    FILEBAR_bg_rect.y = TOPNAV_H;
    FILEBAR_bg_rect.w = WINDOW_W;
    FILEBAR_bg_rect.h = TOPNAV_H;
}

void renderTopNav()
{
    // ===== Top Nav =====
    SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
    SDL_RenderFillRect(renderer, &TOPNAV_bg_rect);

    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);

    SDL_RenderTexture(renderer, logoTexture, NULL, &logoRect);

    int i;

    // Top Nav Left Buttons
    for (i = 0; i < 5; i++)
    {
        TOPNAV_MENU_NODE node = TOPNAV_MENU[i];
        if (node.isActive)
        {
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_FRect r;
            r.x = node.rect.x - 7.9;
            r.w = node.rect.w + 16.8;
            r.y = node.rect.y;
            r.h = node.rect.h;
            SDL_RenderFillRect(renderer, &r);
        }
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderTexture(renderer, node.texture, NULL, &node.rect);
    }

    // TOPNAV_RIGHT Buttons
    for (i = 0; i < 1; i++)
    {
        MENU_BAR_NODE node = TOPNAV_RIGHT[i];
        if (node.isActive)
        {
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_FRect r;
            r.x = node.rect.x - 7.9;
            r.w = node.rect.w + 16.8;
            r.y = node.rect.y - 7.9;
            r.h = node.rect.h + 16.8;
            SDL_RenderFillRect(renderer, &r);
        }
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        if (node.clicked)
            SDL_RenderTexture(renderer, node.active_texture, NULL, &node.rect);
        else
            SDL_RenderTexture(renderer, node.texture, NULL, &node.rect);
    }
}

void renderTopNavBarMenu()
{
    int i;
    // Top Nav Left Buttons
    for (i = 0; i < 5; i++)
    {
        TOPNAV_MENU_NODE node = TOPNAV_MENU[i];
        if (node.clicked == 1)
        {
            int j = 0;
            char *item = node.list[0];
            int prevY = node.rect.y + node.rect.h;
            int itemH = EDITOR_FONT_HEIGHT + 10;
            while (strcmp(item, "") != 0 && j <= 5)
            {
                SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                SDL_FRect r;
                r.x = node.rect.x - 7.9;
                r.y = prevY;
                r.w = 220;
                r.h = itemH;
                SDL_RenderFillRect(renderer, &r);
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);

                SDL_Surface *s1 = TTF_RenderText_Blended(font2, item, strlen(item), (SDL_Color){180, 180, 180, 255});
                SDL_Texture *t1 = SDL_CreateTextureFromSurface(renderer, s1);
                // SDL_SetTextureScaleMode(t1, SDL_SCALEMODE_NEAREST);

                r.x = r.x + 10;
                r.y = r.y + 5;
                r.h = s1->h;
                r.w = (s1->w + 0.0) / s1->h * r.h;

                SDL_RenderTexture(renderer, t1, NULL, &r);
                SDL_DestroyTexture(t1);
                SDL_DestroySurface(s1);

                j++;
                item = node.list[j];
                prevY += itemH;
            }

            // SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_FRect r;
            r.x = node.rect.x - 7.9;
            r.y = node.rect.y + node.rect.h;
            r.w = 220;
            r.h = itemH * j;
            // SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderRect(renderer, &r);
        }
    }
}

/* ===== Left Menu ===== */
void renderMenuBar()
{

    // Left Menu
    SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);

    SDL_RenderFillRect(renderer, &MENUBAR_bg_rect);
    if (showMenu)
        SDL_RenderFillRect(renderer, &MENU_bg_rect);

    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);
    SDL_RenderLine(renderer, 0, TOPNAV_H, 0, WINDOW_H); // Drawing MENUBAR Border
    if (showMenu)
        SDL_RenderLine(renderer, MENU_W, TOPNAV_H, MENU_W, WINDOW_H); // Drawing MENU Border
    SDL_RenderLine(renderer, 0, TOPNAV_H, WINDOW_W, TOPNAV_H);        // Drawing TOPNAV Border

    // ===== Drawing Left Menu Components =====
    if (showMenu)
    {
        if (menu_state == 0)
            renderExplorer();
        if (menu_state == 1)
            renderSearch();
        if (menu_state == 2)
            renderGithub();
        if (menu_state == 3)
            renderExtentions();
    }

    SDL_FRect r = {
        0,
        TOPNAV_H,
        MENU_W,
        LEFT_MENU[0].rect.h + 40};
    // SDL_SetRenderDrawColor(renderer, 50, 50, 130, 255);

    SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
    SDL_RenderFillRect(renderer, &r);

    int i;
    // Menu Bar Buttons
    for (i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
    {
        MENU_BAR_NODE node = LEFT_MENU[i];
        if (menu_state == i)
        {
            SDL_FRect r = {
                0,
                node.rect.y - 10,
                3,
                node.rect.h + 20};
            // SDL_SetRenderDrawColor(renderer, 50, 50, 130, 255);

            SDL_SetRenderDrawColor(renderer, 106, 90, 205, 255);
            SDL_RenderFillRect(renderer, &r);
        }

        if (node.isActive || menu_state == i)
        {
            SDL_RenderTexture(renderer, node.active_texture, NULL, &node.rect);
        }
        else
        {
            SDL_RenderTexture(renderer, node.texture, NULL, &node.rect);
        }
    }
}

void renderFileBar()
{
    if (!FileBar)
        return;

    SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
    SDL_RenderFillRect(renderer, &FILEBAR_bg_rect);

    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);
    SDL_RenderLine(renderer, FILEBAR_bg_rect.x, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h, WINDOW_W, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h);
    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);

    FileBarItem *node = FileBar;
    int i = 0, x = FILEBAR_bg_rect.x;

    while (node != NULL)
    {
        float w, h;
        if (!node->t1)
        {
             SDL_Color color = {239, 239, 239, 255};
             SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->name, strlen(node->name), color);
             if (s1) {
                 node->t1 = SDL_CreateTextureFromSurface(renderer, s1);
                 // SDL_SetTextureScaleMode(node->t1, SDL_SCALEMODE_NEAREST);
                 SDL_DestroySurface(s1);
             }
        }

        if (node->t1) {
             SDL_GetTextureSize(node->t1, &w, &h);

            node->r1.x = x + FILEMENU_SCROLL_X;
            node->r1.y = FILEBAR_bg_rect.y;
            node->r1.w = w + 40;
            node->r1.h = FILEBAR_bg_rect.h;

            node->r2.x = x + node->r1.w / 2 - w / 2 + FILEMENU_SCROLL_X;
            node->r2.y = FILEBAR_bg_rect.y + FILEBAR_bg_rect.h / 2 - h / 2;
            node->r2.w = w;
            node->r2.h = h;

            x += node->r1.w;

            if (node == currentActiveTag)
            {
                SDL_SetRenderDrawColor(renderer, 66, 66, 66, 100);
                SDL_RenderFillRect(renderer, &node->r1);
            }

            SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);
            SDL_RenderLine(renderer, node->r1.x + node->r1.w, FILEBAR_bg_rect.y, node->r1.x + node->r1.w, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h);

            SDL_RenderTexture(renderer, node->t1, NULL, &node->r2);
        }

        node = node->next;
        i += 1;
    }
}

void renderTextEditor()
{

    FileBarItem *node = currentActiveTag;

    if (!node)
    {
        // SDL_RenderTexture(renderer, logoTexture2, NULL, &logoRect2);
        return;
    }

    if (node->type == AETHER_IMG)
    {
        if (!node->t2)
        {
            SDL_Surface *s1 = IMG_Load(node->path);
            if (s1)
            {
                node->t2 = SDL_CreateTextureFromSurface(renderer, s1);
                // SDL_SetTextureScaleMode(node->t2, SDL_SCALEMODE_NEAREST);
                SDL_DestroySurface(s1);
            }
        }

        if (node->t2)
        {
            float w, h;
            SDL_GetTextureSize(node->t2, &w, &h);

            int editorW = WINDOW_W - MENU_W - 50;
            int editorH = WINDOW_H - 2 * TOPNAV_H - FOOTER_H - 50;
            SDL_FRect r1;

            if (w > editorW || h > editorH)
            {
                if (w > h)
                {
                    r1.w = editorW;
                    r1.h = ((h + 0.0) / w) * editorW;
                }
                else
                {
                    r1.h = editorH;
                    r1.w = ((w + 0.0) / h) * editorH;
                }
            }
            else
            {
                r1.w = w;
                r1.h = h;
            }

            r1.x = MENU_W + editorW / 2 + 25 - r1.w / 2;
            r1.y = 2 * TOPNAV_H + editorH / 2 + 25 - r1.h / 2;

            SDL_RenderTexture(renderer, node->t2, NULL, &r1);
        }
        return;
    }

    FileLine *line = node->lines;
    if (!line)
        return;

    int y = 0;
    int lineNo = 0;

    SDL_FRect minimapBgRect = {
        WINDOW_W - 100,
        FILEBAR_bg_rect.y + FILEBAR_bg_rect.h,
        MINIMAP_W,
        WINDOW_H};

    // SDL_Color fg = {255, 255, 255, 255};

    int hasSelectionStarted = 0;
    int LINE_NUMBER_WIDTH = 70;
    while (line)
    {
        Token *word = line->word;
        int isCollapsed = line->collapsed;

        if (!word)
        {
            Token *t = createToken("", 1, (SDL_Color){200, 255, 200, 255});
            t->next = NULL;
            t->prev = NULL;
            line->word = t;
            word = t;
        }

        if(isCollapsed == 1){
            int n = 1;
            int exit = 0;
            while (!exit)
            {
                line = line->next;
                lineNo++;
                if(line->collapsed == 1){
                    n+=1;
                }

                if(line->collapsed == 2){
                    n-=1;
                }

                if(n == 0){
                    exit = 1;
                    break;
                }
            }
            
        }

        int x = 0;

        if (currentActiveTag->currentWord != currentActiveTag->SELECTION_START_WORD && (line == currentActiveTag->currentLine || line == currentActiveTag->SELECTION_START_LINE))
        {
            hasSelectionStarted = !hasSelectionStarted;
        }

        if ((y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * EDITOR_FONT_HEIGHT == 0)
        {
            currentActiveTag->visibleLine = line;
        }

        if ((y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * EDITOR_FONT_HEIGHT < 0 || y * EDITOR_FONT_SIZE > -currentActiveTag->EDITOR_SCROLL_Y + WINDOW_H)
        {
            y++;
            lineNo++;
            line = line->next;
            continue;
        }

        while (word)
        {
            if(!word) break;
            if(!word->content){
                word = word->next;
                continue;
            }
            word->len = strlen(word->content);
            // printf("%s",word->content);

            float w = 0, h = 0;
            SDL_GetTextureSize(word->t1, &w, &h);
            h = EDITOR_FONT_HEIGHT;
            SDL_FRect r1 = {
                LINE_NUMBER_WIDTH + FILEBAR_bg_rect.x + 4 + currentActiveTag->EDITOR_SCROLL_X + x,
                FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * h,
                w,
                h};

            /* ===== Draw Cursor ===== */
            if (word == currentActiveTag->currentWord)
            {
                int x = r1.x + cursor->w * currentActiveTag->startIndex;
                cursor->x = x;
                cursor->y = r1.y;
                SDL_FRect cursorRect = {x, r1.y, 2, cursor->h};

                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderFillRect(renderer, &cursorRect);
            }

            /*
            if (word->len == 0 && word->prev)
            {
                if(currentActiveTag->currentWord == word){
                    // if(word->next){
                    //     currentActiveTag->currentWord = word->next;
                    //     currentActiveTag->startIndex = 0;
                    // } else {
                        currentActiveTag->currentWord = word->prev;
                        currentActiveTag->startIndex = word->len;
                    // }
                }
                Token *temp = word;
                if(word->prev) word->prev->next = word->next;
                if(word->next) word->next->prev = word->prev;
                word = word->next;

                SDL_DestroyTexture(temp->t1);
                free(temp);

                continue;
            }
            */

            if (r1.x + r1.w > MENU_W && r1.x < RIGHTPANEL_X)
            {
                SDL_RenderTexture(renderer, word->t1, NULL, &r1);
            }
            // printf("[%s]", word->content);
            x += w;
            word = word->next;
        }


        if(isCollapsed){
            SDL_FRect highlightRect = {
                MENU_W + LINE_NUMBER_WIDTH,
                FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * EDITOR_FONT_HEIGHT,
                WINDOW_W,
                EDITOR_FONT_HEIGHT};
                
            SDL_SetRenderDrawColor(renderer, 200, 40, 200, 30);
            SDL_RenderFillRect(renderer, &highlightRect);
        }

        if (hasSelectionStarted)
        {
            SDL_FRect selectionRect = {
                MENU_W + LINE_NUMBER_WIDTH,
                FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * EDITOR_FONT_HEIGHT,
                WINDOW_W,
                EDITOR_FONT_HEIGHT};

            SDL_SetRenderDrawColor(renderer, 100, 150, 250, 50);
            SDL_RenderFillRect(renderer, &selectionRect);
        }

        // printf("\n");
        /*===== Draw Line Number =====*/
        char text[12];
        sprintf(text, "%d", lineNo + 1);
        int len = strlen(text);
        int startX = FILEBAR_bg_rect.x + LINE_NUMBER_WIDTH / 2 - (len * DIGIT_W) / 2;
        int startY = FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * EDITOR_FONT_HEIGHT;

        SDL_FRect lineNoBgRect = {
            MENU_W,
            startY,
            LINE_NUMBER_WIDTH,
            EDITOR_FONT_HEIGHT};

        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
        if(isCollapsed) SDL_SetRenderDrawColor(renderer, 200, 40, 200, 30);
        SDL_RenderFillRect(renderer, &lineNoBgRect);

        for (int k = 0; k < len; k++)
        {
            int digit = text[k] - '0';
            if (digit >= 0 && digit <= 9 && DIGIT_TEXTURES[digit])
            {
                SDL_FRect digitRect = {
                    startX + k * DIGIT_W,
                    startY,
                    DIGIT_W,
                    DIGIT_H};
                SDL_RenderTexture(renderer, DIGIT_TEXTURES[digit], NULL, &digitRect);
            }
        }

        y++;
        lineNo++;
        line = line->next;
    }

    /* ===== Render Minimap =====*/
    if (currentActiveTag->length < 1000000)
    {
        if (TOPNAV_RIGHT[0].clicked)
            minimapBgRect.x = RIGHTPANEL_X - minimapBgRect.w;

        // Count lines for texture height
        int totalLines = 0;
        FileLine *tempLine = node->lines;
        while (tempLine)
        {
            totalLines++;
            tempLine = tempLine->next;
        }

        int textureH = totalLines * 2;
        if (textureH > 8192) textureH = 8192; // Limit max height
        if (textureH < 2) textureH = 2;

        if (currentActiveTag->minimapDirty || !currentActiveTag->minimapTexture)
        {
            if (currentActiveTag->minimapTexture)
            {
                SDL_DestroyTexture(currentActiveTag->minimapTexture);
                currentActiveTag->minimapTexture = NULL;
            }

            currentActiveTag->minimapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, MINIMAP_W, textureH);
            if (currentActiveTag->minimapTexture)
            {
                SDL_SetTextureBlendMode(currentActiveTag->minimapTexture, SDL_BLENDMODE_BLEND);
                SDL_SetRenderTarget(renderer, currentActiveTag->minimapTexture);

                SDL_SetRenderDrawColor(renderer, 17, 17, 17, 250);
                SDL_RenderClear(renderer);

                FileLine *mLine = node->lines;
                int mY = 0;
                while (mLine && mY * 2 < textureH)
                {
                    Token *wrd = mLine->word;
                    int i = 0;
                    while (wrd)
                    {
                        if (strcmp(wrd->content, " ") == 0)
                        {
                            i++;
                            wrd = wrd->next;
                            continue;
                        }
                        SDL_FRect minimapRect = {
                            4.0f + i,
                            mY * 2.0f,
                            (float)strlen(wrd->content),
                            1.5f};
                        
                        if (wrd->color.r >= 200 && wrd->color.g >= 200 && wrd->color.b >= 200)
                            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
                        else
                            SDL_SetRenderDrawColor(renderer, wrd->color.r, wrd->color.g, wrd->color.b, wrd->color.a);
                        
                        SDL_RenderFillRect(renderer, &minimapRect);
                        
                        i += (int)strlen(wrd->content);
                        wrd = wrd->next;
                    }
                    mY++;
                    mLine = mLine->next;
                }
                SDL_SetRenderTarget(renderer, NULL);
                currentActiveTag->minimapDirty = 0;
            }
        }

        if (currentActiveTag->minimapTexture)
        {
             SDL_FRect dstRect = {
                minimapBgRect.x,
                FILEBAR_bg_rect.y + FILEBAR_bg_rect.h,
                MINIMAP_W,
                (float)textureH
            };
            SDL_RenderTexture(renderer, currentActiveTag->minimapTexture, NULL, &dstRect);
        }

        // Calculate visible index for highlight
        int visibleIndex = 0;
        FileLine* vLine = node->lines;
        while(vLine) {
            if(vLine == currentActiveTag->visibleLine) break;
            visibleIndex++;
            vLine = vLine->next;
        }

        int highlightStartY = FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + visibleIndex * 2; 

        /* ===== Draw Highlight Rect ===== */
        SDL_FRect highlightRect = {
            minimapBgRect.x,
            highlightStartY,
            MINIMAP_W,
            (WINDOW_H - 3 * TOPNAV_H) / EDITOR_FONT_HEIGHT * 2};

        SDL_SetRenderDrawColor(renderer, 137, 137, 137, 100);
        SDL_RenderFillRect(renderer, &highlightRect);

        /*===== Draw Borders =====*/
        SDL_SetRenderDrawColor(renderer, 57, 57, 57, 255);
        SDL_RenderLine(renderer, minimapBgRect.x, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h, minimapBgRect.x, WINDOW_H);
    }

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderLine(renderer, FILEBAR_bg_rect.x + LINE_NUMBER_WIDTH, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h, FILEBAR_bg_rect.x + LINE_NUMBER_WIDTH, WINDOW_H);
}

void renderRightPanel()
{
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 250);
    SDL_FRect r1 = {
        RIGHTPANEL_X,
        TOPNAV_H,
        RIGHTPANEL_W,
        WINDOW_H - TOPNAV_H};

    SDL_RenderFillRect(renderer, &r1);

    /*===== Draw Borders =====*/
    SDL_SetRenderDrawColor(renderer, 57, 57, 57, 255);
    SDL_RenderLine(renderer, RIGHTPANEL_X, FILEBAR_bg_rect.y, RIGHTPANEL_X, WINDOW_H);
}

void renderSuggestionBox()
{
    if (!currentActiveTag || !currentActiveTag->currentWord || !currentActiveTag->active)
        return;

    if (showCompletion && cursor && cursor->x != -1 && cursor->y != -1)
    {
        CompletionListItem *node = CompletionBox.list;
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        int y = cursor->y + cursor->h;
        int padding = 6;
        while (node)
        {
            float w, h;
            SDL_GetTextureSize(node->t1, &w, &h);

            SDL_FRect bgRect = {cursor->x, y, 300, cursor->h + padding};
            if (node == CompletionBox.active)
            {
                // SDL_SetRenderDrawColor(renderer, 255, 70, 180, 255);
                SDL_SetRenderDrawColor(renderer, 196, 53, 139, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            }
            SDL_RenderFillRect(renderer, &bgRect);

            SDL_FRect textRect = {cursor->x + padding, y + cursor->h / 2 - h / 2, w, h};
            SDL_RenderTexture(renderer, node->t1, NULL, &textRect);

            node = node->next;
            y += h + padding;
        }

        if (y - cursor->y - cursor->h > 0)
        {
            SDL_FRect completionRect = {cursor->x, cursor->y + cursor->h, 300, y - cursor->y - cursor->h + padding / 2};
            SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
            SDL_RenderRect(renderer, &completionRect);
        }
        else
        {
            showCompletion = 0;
        }
    }
}

void renderExplorer()
{

    MENU_BAR_NODE node = LEFT_MENU[0];

    SDL_Color color = {180, 180, 180, 255};

    // Initialize Explorer if not
    // Textures
    if (!Explorer.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, "You have not opened", strlen("You have not opened"), color);
        Explorer.t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(Explorer.t1, SDL_SCALEMODE_NEAREST);
    }

    if (!Explorer.t2)
    {
        SDL_Surface *s2 = TTF_RenderText_Blended(poppins_regular, "any folder yet", strlen("any folder yet"), color);
        Explorer.t2 = SDL_CreateTextureFromSurface(renderer, s2);
        // SDL_SetTextureScaleMode(Explorer.t2, SDL_SCALEMODE_NEAREST);
    }

    if (!Explorer.t3)
    {
        SDL_Color color = {233, 233, 233, 255};
        SDL_Surface *s3 = TTF_RenderText_Blended(font2, "Open Folder", strlen("Open Folder"), color);
        Explorer.t3 = SDL_CreateTextureFromSurface(renderer, s3);
        // SDL_SetTextureScaleMode(Explorer.t3, SDL_SCALEMODE_NEAREST);
    }

    if (!Explorer.t4)
    {
        SDL_Surface *s4 = TTF_RenderText_Blended(poppins_regular, "Open Recent", strlen("Open Recent"), color);
        Explorer.t4 = SDL_CreateTextureFromSurface(renderer, s4);
        // SDL_SetTextureScaleMode(Explorer.t4, SDL_SCALEMODE_NEAREST);
    }

    // Rects
    if (!Explorer.r1.x)
    {
        float w, h;
        SDL_GetTextureSize(Explorer.t1, &w, &h);
        SDL_FRect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h};
        Explorer.r1 = r1; // First Text Box
    }

    if (!Explorer.r2.x)
    {
        float w, h;
        SDL_GetTextureSize(Explorer.t2, &w, &h);
        SDL_FRect r2 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + Explorer.r1.h,
            w,
            h};
        Explorer.r2 = r2; // Second Text Box
    }

    if (!Explorer.r3.x)
    {
        float w, h;
        SDL_GetTextureSize(Explorer.t1, &w, &h);
        SDL_FRect r3 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3 * MENU_PAD_Y + Explorer.r1.h + Explorer.r2.h - 2.5,
            MENU_W - 2 * MENU_PAD_Y,
            h + 5};
        Explorer.r3 = r3; // First Button Container
    }

    if (!Explorer.r4.x)
    {
        float w, h;
        SDL_GetTextureSize(Explorer.t3, &w, &h);
        SDL_FRect r4 = {
            MENU_W / 2 - w / 2,
            node.text_rect.y + node.text_rect.h + 3 * MENU_PAD_Y + Explorer.r1.h + Explorer.r2.h,
            w,
            h};
        Explorer.r4 = r4; // First Button Text Container
    }

    int i = 0;
    SDL_SetRenderDrawColor(renderer, 106, 90, 205, 255);

    if (!Folder)
    {
        SDL_RenderFillRect(renderer, &Explorer.r3);
        SDL_RenderTexture(renderer, Explorer.t1, NULL, &Explorer.r1);
        SDL_RenderTexture(renderer, Explorer.t2, NULL, &Explorer.r2);
        SDL_RenderTexture(renderer, Explorer.t3, NULL, &Explorer.r4);
    }
    else
    {
        renderFolder(&Folder, &i, 0);
    }

    // Display the Menu Name
    if (!Folder)
    {
        SDL_RenderTexture(renderer, node.text_texture, NULL, &node.text_rect);
    }
    else
    {
        if (!Folder->t1)
        {
            SDL_Color color = {155, 155, 155, 255};
            SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, Folder->name, strlen(Folder->name), color);
            Folder->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            // SDL_SetTextureScaleMode(Folder->t1, SDL_SCALEMODE_NEAREST);
            SDL_DestroySurface(s1);
        }

        SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
        SDL_FRect BG = {
            1,
            TOPNAV_H + MENU_BAR_H,
            MENU_W - 1,
            Folder->r1.h * 2};
        SDL_RenderFillRect(renderer, &BG);
        // SDL_SetRenderDrawColor(renderer, 56, 56, 56, 255);
        // SDL_RenderLine(renderer, BG.x, BG.y, BG.x + BG.w, BG.y);
        SDL_RenderTexture(renderer, Folder->t1, NULL, &Folder->r1);
    }
}

void renderFolder(FileNode **folder, int *i, int padX)
{

    if (!(*folder)->child)
    {
        if (!(*folder)->isDirOpened)
        {

            populateFolder(*folder);
        }
        else
        {
            return;
        }
    }

    FileNode *node = (*folder)->child;

    while (node != NULL)
    {
        // printf("%s ", node->name);
        SDL_SetRenderDrawColor(renderer, 206, 206, 206, 255);
        // SDL_RenderFillRect(renderer, &node->r1);

        float w, h;
        if (!node->t1)
        {
            // SDL_Color color = {239, 239, 239, 255};
            SDL_Color color = {200, 200, 200, 255};
            SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->name, strlen(node->name), color);
            if (s1) {
                node->t1 = SDL_CreateTextureFromSurface(renderer, s1);
                // SDL_SetTextureScaleMode(node->t1, SDL_SCALEMODE_NEAREST);
                SDL_DestroySurface(s1);
            }
        }

        if (node->t1) {
            SDL_GetTextureSize(node->t1, &w, &h);

            node->r1.x = padX * 10 + LEFT_MENU[0].rect.x + h + 2;
            node->r1.w = w;
            node->r1.y = MENU_BAR_H + LEFT_MENU[0].rect.y + LEFT_MENU[0].rect.h + h * (*i) + MENU_PAD_Y / 2 * (*i) + EXPLORER_SCROLL_Y;
            node->r1.h = h;
        }

        if (node->hovered)
        {
            SDL_FRect hoverRect = {
                MENU_PAD_X,
                node->r1.y,
                MENU_W - 2 * MENU_PAD_X,
                node->r1.h};

            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
            SDL_RenderFillRect(renderer, &hoverRect);
        }

        if (currentActiveTag && strcmp(currentActiveTag->path, node->path) == 0)
        {
            SDL_FRect borderRect = {
                MENU_PAD_X,
                node->r1.y,
                MENU_W - 2 * MENU_PAD_X,
                node->r1.h};

            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
            SDL_RenderFillRect(renderer, &borderRect);
        }

        FileIcons.r1.x = node->r1.x - 2 - node->r1.h;
        FileIcons.r1.y = node->r1.y + 2;
        FileIcons.r1.w = node->r1.h - 4;
        FileIcons.r1.h = node->r1.h - 4;

        if (node->isDir)
        {
            if (node->opened)
            {
                SDL_RenderTexture(renderer, FileIcons.t3, NULL, &FileIcons.r1); // Display Folder Open Icon
                (*i) += 1;
                renderFolder(&node, i, padX + 1);
                SDL_RenderTexture(renderer, node->t1, NULL, &node->r1);
                node = node->next;
                continue;
            }
            else
                SDL_RenderTexture(renderer, FileIcons.t2, NULL, &FileIcons.r1); // Display Folder Closed Icon
        }
        else
        {
            // Display File Icons
            SDL_Texture *fileTexture = FileIcons.t1;
            if (node->type == AETHER_CLANG)
                fileTexture = FileIcons.t4;
            else if (node->type == AETHER_CHEADER)
                fileTexture = FileIcons.t4;
            else if (node->type == AETHER_HTML)
                fileTexture = FileIcons.t5;
            else if (node->type == AETHER_CSS)
                fileTexture = FileIcons2.t1;
            else if (node->type == AETHER_IMG)
                fileTexture = FileIcons2.t2;

            SDL_RenderTexture(renderer, fileTexture, NULL, &FileIcons.r1);
        }

        SDL_RenderTexture(renderer, node->t1, NULL, &node->r1); // Display File/Folder name
        node = node->next;
        (*i) += 1;
    }
}

void renderSearch()
{
    MENU_BAR_NODE node = LEFT_MENU[1];
    SDL_RenderTexture(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {100, 100, 100, 255};

    // Textures
    if (!Search.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, SearchMenu.txt1, strlen(SearchMenu.txt1), color);
        Search.t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(Search.t1, SDL_SCALEMODE_NEAREST);
    }

    if (!Search.t2)
    {
        SDL_Surface *s2 = TTF_RenderText_Blended(poppins_regular, SearchMenu.txt2, strlen(SearchMenu.txt2), color);
        Search.t2 = SDL_CreateTextureFromSurface(renderer, s2);
        // SDL_SetTextureScaleMode(Search.t2, SDL_SCALEMODE_NEAREST);
    }

    // Rects

    if (!Search.r1.x)
    {
        float w, h;
        SDL_GetTextureSize(Search.t1, &w, &h);
        SDL_FRect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            MENU_W - 2 * MENU_PAD_X,
            h + 5};
        Search.r1 = r1;
    }

    if (!Search.r2.x)
    {
        float w, h;
        SDL_GetTextureSize(Search.t1, &w, &h);
        SDL_FRect r2 = {
            MENU_PAD_X + 10,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + 2.5,
            w,
            h};
        Search.r2 = r2;
    }

    if (!Search.r3.x)
    {
        float w, h;
        SDL_GetTextureSize(Search.t2, &w, &h);
        SDL_FRect r3 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3.6 / 2 * MENU_PAD_Y + Search.r1.h,
            MENU_W - 2 * MENU_PAD_X,
            h + 5};
        Search.r3 = r3;
    }

    if (!Search.r4.x)
    {
        float w, h;
        SDL_GetTextureSize(Search.t2, &w, &h);
        SDL_FRect r4 = {
            MENU_PAD_X + 10,
            node.text_rect.y + node.text_rect.h + 3.6 / 2 * MENU_PAD_Y + 2.5 + Search.r1.h,
            w,
            h};
        Search.r4 = r4;
    }

    SDL_SetRenderDrawColor(renderer, 6, 6, 6, 255);
    SDL_RenderFillRect(renderer, &Search.r1);
    SDL_RenderFillRect(renderer, &Search.r3);
    SDL_RenderTexture(renderer, Search.t1, NULL, &Search.r2);
    SDL_RenderTexture(renderer, Search.t2, NULL, &Search.r4);
}

void renderGithub()
{
    MENU_BAR_NODE node = LEFT_MENU[2];
    SDL_RenderTexture(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {180, 180, 180, 255};

    // Textures
    if (!Github.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, "Import From a GitHub", strlen("Import From a GitHub"), color);
        Github.t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(Github.t1, SDL_SCALEMODE_NEAREST);
    }

    if (!Github.t2)
    {
        SDL_Surface *s2 = TTF_RenderText_Blended(poppins_regular, "Repository", strlen("Repository"), color);
        Github.t2 = SDL_CreateTextureFromSurface(renderer, s2);
        // SDL_SetTextureScaleMode(Github.t2, SDL_SCALEMODE_NEAREST);
    }

    if (!Github.t3)
    {
        SDL_Color color = {233, 233, 233, 255};
        SDL_Surface *s3 = TTF_RenderText_Blended(font2, "Connect Github", strlen("Connect Github"), color);
        Github.t3 = SDL_CreateTextureFromSurface(renderer, s3);
        // SDL_SetTextureScaleMode(Github.t3, SDL_SCALEMODE_NEAREST);
    }

    // if(!Github.t4){
    //     SDL_Surface* s4 = TTF_RenderText_Blended(poppins_regular, "Open Recent", color);
    //     Github.t4 = SDL_CreateTextureFromSurface(renderer, s4);
    //     //SDL_SetTextureScaleMode(Github.t4, SDL_SCALEMODE_NEAREST);
    // }

    // Rects
    if (!Github.r1.x)
    {
        float w, h;
        SDL_GetTextureSize(Github.t1, &w, &h);
        SDL_FRect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h};
        Github.r1 = r1; // First Text Box
    }

    if (!Github.r2.x)
    {
        float w, h;
        SDL_GetTextureSize(Github.t2, &w, &h);
        SDL_FRect r2 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + Github.r1.h,
            w,
            h};
        Github.r2 = r2; // Second Text Box
    }

    if (!Github.r3.x)
    {
        float w, h;
        SDL_GetTextureSize(Github.t1, &w, &h);
        SDL_FRect r3 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3 * MENU_PAD_Y + Github.r1.h + Github.r2.h - 2.5,
            MENU_W - 2 * MENU_PAD_Y,
            h + 5};
        Github.r3 = r3; // First Button Container
    }

    if (!Github.r4.x)
    {
        float w, h;
        SDL_GetTextureSize(Github.t3, &w, &h);
        SDL_FRect r4 = {
            MENU_W / 2 - w / 2,
            node.text_rect.y + node.text_rect.h + 3 * MENU_PAD_Y + Github.r1.h + Github.r2.h,
            w,
            h};
        Github.r4 = r4; // First Button Text Container
    }

    SDL_SetRenderDrawColor(renderer, 106, 90, 205, 255);
    SDL_RenderFillRect(renderer, &Github.r3);
    SDL_RenderTexture(renderer, Github.t1, NULL, &Github.r1);
    SDL_RenderTexture(renderer, Github.t2, NULL, &Github.r2);
    SDL_RenderTexture(renderer, Github.t3, NULL, &Github.r4);
}

void renderExtentions()
{
    MENU_BAR_NODE node = LEFT_MENU[3];
    SDL_RenderTexture(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {130, 130, 130, 255};

    // Textures
    if (!Extentions.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, "Feature Coming Soon", strlen("Feature Coming Soon"), color);
        Extentions.t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(Extentions.t1, SDL_SCALEMODE_NEAREST);
    }

    // Rects
    if (!Extentions.r1.x)
    {
        float w, h;
        SDL_GetTextureSize(Extentions.t1, &w, &h);
        SDL_FRect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h};
        Extentions.r1 = r1; // First Text Box
    }

    SDL_RenderTexture(renderer, Extentions.t1, NULL, &Extentions.r1);
}

void renderFooter()
{
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
    SDL_FRect r1 = {
        0,
        WINDOW_H - FOOTER_H,
        WINDOW_W,
        FOOTER_H};
    SDL_RenderFillRect(renderer, &r1);
    SDL_SetRenderDrawColor(renderer, 47, 47, 47, 255);
    SDL_RenderLine(renderer, r1.x, r1.y, r1.x + r1.w, r1.y);

    if (!Footer.t1)
    {
        // SDL_Surface* s1 = TTF_RenderText_Blended(jetbrains_regular, "open source @ github.com/7777satish/aether", (SDL_Color){100, 100, 100, 255});
        SDL_Surface *s1 = TTF_RenderText_Blended(jetbrains_regular, "/7777Satish/Aether", strlen("/7777Satish/Aether"), (SDL_Color){140, 140, 140, 255});
        Footer.t1 = SDL_CreateTextureFromSurface(renderer, s1);
        // SDL_SetTextureScaleMode(Footer.t1, SDL_SCALEMODE_NEAREST);
        SDL_DestroySurface(s1);
    }

    SDL_FRect r = {
        10,
        WINDOW_H - FOOTER_H / 2 - (FOOTER_H - 15) / 2,
        (LEFT_MENU[2].rect.w + 0.0) / LEFT_MENU[2].rect.h * (FOOTER_H - 15),
        (FOOTER_H - 15)};

    if (!Footer.r1.x)
    {
        float w, h;
        SDL_GetTextureSize(Footer.t1, &w, &h);

        SDL_FRect r1 = {
            r.x + r.w + 5,
            WINDOW_H - FOOTER_H / 2 - (FOOTER_H - 15) / 2,
            ((w + 0.0) / h) * (FOOTER_H - 15),
            (FOOTER_H - 15)};

        Footer.r1 = r1;
    }

    SDL_RenderTexture(renderer, LEFT_MENU[2].texture, NULL, &r);
    SDL_RenderTexture(renderer, Footer.t1, NULL, &Footer.r1);
}
