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

// Fonts
TTF_Font *poppins_regular = NULL;
TTF_Font *poppins_bold = NULL;
TTF_Font *font2 = NULL;
TTF_Font *jetbrains_regular = NULL;

// Creating Top Nav and Left Menu
SDL_Rect TOPNAV_bg_rect = {};
SDL_Rect MENUBAR_bg_rect = {};
SDL_Rect MENU_bg_rect = {};
SDL_Rect FILEBAR_bg_rect = {};

/* ===== Initialising Logos ===== */

SDL_Surface *logoSurface2 = NULL;
SDL_Texture *logoTexture2 = NULL;
int logo2AW, logo2AH;
int dlw2 = 300;
int dlh2;
SDL_Rect logoRect2;

SDL_Surface *logoSurface = NULL;
SDL_Texture *logoTexture = NULL;
int logoAW, logoAH;
SDL_Rect logoRect = {};

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

    // Initialize SDL2 video subsystem with OpenGL support
    SDL_Init(SDL_VIDEO_OPENGL);
    // Initialize TrueType font rendering subsystem
    TTF_Init();
    // Initialize image loading subsystem (PNG, JPG support)
    IMG_Init(0);

    // Enable high-quality scaling for textures
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Create main application window
    window = SDL_CreateWindow("CodeEditor_C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
    // Create hardware-accelerated renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // EDITOR_FONT_SIZE = TOP_NAV_LOGO_H / 1.7;
    EDITOR_FONT_SIZE = TOP_NAV_LOGO_H / 1.5;

    /* ===== Initialising Fonts ===== */
    poppins_regular = TTF_OpenFont("assets/Poppins/Poppins-Regular.ttf", TOP_NAV_LOGO_H / 1.7);
    if (!poppins_regular)
    {
        fprintf(stderr, "Failed to load Poppins-Regular.ttf: %s\n", TTF_GetError());
        exit(1);
    }

    poppins_bold = TTF_OpenFont("assets/Poppins/Poppins-Bold.ttf", TOP_NAV_LOGO_H / 1.7);
    if (!poppins_bold)
    {
        fprintf(stderr, "Failed to load Poppins-Bold.ttf: %s\n", TTF_GetError());
        exit(1);
    }

    font2 = TTF_OpenFont("assets/Montserrat/static/Montserrat-Regular.ttf", TOP_NAV_LOGO_H / 1.7 - 1);
    if (!font2)
    {
        fprintf(stderr, "Failed to load Montserrat-Regular.ttf: %s\n", TTF_GetError());
        exit(1);
    }

    jetbrains_regular = TTF_OpenFont("assets/JetBrains_Mono./static/JetBrainsMono-Regular.ttf", EDITOR_FONT_SIZE);
    if (!jetbrains_regular)
    {
        fprintf(stderr, "Failed to load JetBrainsMono-Regular.ttf: %s\n", TTF_GetError());
        exit(1);
    }

    /* ===== Initialising Cursor ===== */

    SDL_Surface *space = TTF_RenderText_Blended(jetbrains_regular, " ", (SDL_Color){0, 0, 0, 255});

    cursor = (Cursor *)malloc(sizeof(Cursor));
    cursor->w = space->w;
    cursor->h = space->h;
    EDITOR_FONT_HEIGHT = space->h;
    SDL_FreeSurface(space);

    /* ===== Initialising Logos ===== */

    logoSurface2 = IMG_Load("assets/logo_grayscale.png");
    logoTexture2 = SDL_CreateTextureFromSurface(renderer, logoSurface2);
    SDL_FreeSurface(logoSurface2);
    SDL_QueryTexture(logoTexture2, NULL, NULL, &logo2AW, &logo2AH);
    dlw2 = 300;
    dlh2 = (logo2AH + 0.0) / logo2AW * dlw2;

    logoRect2.x = MENU_W + (WINDOW_W - MENU_W) / 2 - dlw2 / 2;
    logoRect2.y = TOPNAV_H + (WINDOW_H - TOPNAV_H) / 2 - dlh2 / 2;
    logoRect2.w = dlw2;
    logoRect2.h = dlh2;

    /* NavBar Logo Start */
    logoSurface = IMG_Load("assets/logo.png");
    logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    int logoAW = logoSurface->w, logoAH = logoSurface->h;
    SDL_FreeSurface(logoSurface);
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

        SDL_Surface *node_surface = TTF_RenderText_Blended(poppins_regular, node->name, color);
        if (!node_surface)
        {
            fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
            continue;
        }

        node->texture = SDL_CreateTextureFromSurface(renderer, node_surface);
        if (!node->texture)
        {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(node_surface);
            continue;
        }

        SDL_Rect rect = {
            prevX,
            TOPNAV_H / 2 - node_surface->h / 2,
            node_surface->w,
            node_surface->h};
        node->rect = rect;

        prevX += node_surface->w + TOPNAV_PADDINGX;

        SDL_FreeSurface(node_surface);
    }

    // Menu Bar Buttons
    prevX = 0;
    for (int i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
    {
        MENU_BAR_NODE *node = &LEFT_MENU[i];
        SDL_Color color = {155, 155, 155, 255};

        SDL_Surface *node_surface = IMG_Load(node->icon);
        SDL_Surface *node_active_surface = IMG_Load(node->active_icon);
        SDL_Surface *node_text_surface = TTF_RenderText_Blended(poppins_regular, node->name, color);
        if (!node_surface || !node_active_surface || !node_text_surface)
        {
            fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
            continue;
        }

        node->texture = SDL_CreateTextureFromSurface(renderer, node_surface);
        node->active_texture = SDL_CreateTextureFromSurface(renderer, node_active_surface);
        node->text_texture = SDL_CreateTextureFromSurface(renderer, node_text_surface);
        if (!node->texture || !node->active_texture || !node->text_texture)
        {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(node_surface);
            continue;
        }

        int viewW = MENU_BAR_H - 30;
        int viewH = (viewW) * (node_surface->h + 0.0) / node_surface->w;

        SDL_Rect rect = {
            MENU_BAR_H / 2 - viewW / 2 + prevX,
            TOPNAV_H + viewH,
            viewW,
            viewH};
        node->rect = rect;

        SDL_Rect rect2 = {
            MENU_PAD_X,
            TOPNAV_H + MENU_BAR_H + MENU_PAD_Y,
            node_text_surface->w,
            node_text_surface->h};

        node->text_rect = rect2;

        prevX += viewW + 20;

        SDL_FreeSurface(node_surface);
    }

    // Initializing FileIcons
    SDL_Surface *fi_s1 = IMG_Load("assets/file_icon2.png");
    FileIcons.t1 = SDL_CreateTextureFromSurface(renderer, fi_s1);
    FileIcons.r1.w = fi_s1->w;
    FileIcons.r1.h = fi_s1->h;
    SDL_FreeSurface(fi_s1);

    SDL_Surface *fi_s2 = IMG_Load("assets/chevron_right.png");
    FileIcons.t2 = SDL_CreateTextureFromSurface(renderer, fi_s2);
    FileIcons.r2.w = fi_s2->w;
    FileIcons.r2.h = fi_s2->h;
    SDL_FreeSurface(fi_s2);

    SDL_Surface *fi_s3 = IMG_Load("assets/chevron_down.png");
    FileIcons.t3 = SDL_CreateTextureFromSurface(renderer, fi_s3);
    FileIcons.r3.w = fi_s3->w;
    FileIcons.r3.h = fi_s3->h;
    SDL_FreeSurface(fi_s3);

    SDL_Surface *fi_s4 = IMG_Load("assets/file_icons/icon_clang.png");
    FileIcons.t4 = SDL_CreateTextureFromSurface(renderer, fi_s4);
    FileIcons.r4.w = fi_s4->w;
    FileIcons.r4.h = fi_s4->h;
    SDL_FreeSurface(fi_s4);

    SDL_Surface *fi_s5 = IMG_Load("assets/file_icons/icon_html.png");
    FileIcons.t5 = SDL_CreateTextureFromSurface(renderer, fi_s5);
    FileIcons.r5.w = fi_s5->w;
    FileIcons.r5.h = fi_s5->h;
    SDL_FreeSurface(fi_s5);

    SDL_Surface *fi2_s1 = IMG_Load("assets/file_icons/icon_css.png");
    FileIcons2.t1 = SDL_CreateTextureFromSurface(renderer, fi2_s1);
    FileIcons2.r1.w = fi2_s1->w;
    FileIcons2.r1.h = fi2_s1->h;
    SDL_FreeSurface(fi2_s1);

    SDL_Surface *fi2_s2 = IMG_Load("assets/file_icons/icon_img.png");
    FileIcons2.t2 = SDL_CreateTextureFromSurface(renderer, fi2_s2);
    FileIcons2.r2.w = fi2_s2->w;
    FileIcons2.r2.h = fi2_s2->h;
    SDL_FreeSurface(fi2_s2);

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

    SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);

    int i;

    // Top Nav Left Buttons
    for (i = 0; i < 5; i++)
    {
        TOPNAV_MENU_NODE node = TOPNAV_MENU[i];
        if (node.isActive)
        {
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_Rect r;
            r.x = node.rect.x - 7.9;
            r.w = node.rect.w + 16.8;
            r.y = node.rect.y;
            r.h = node.rect.h;
            SDL_RenderFillRect(renderer, &r);
        }
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderCopy(renderer, node.texture, NULL, &node.rect);
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
                SDL_Rect r;
                r.x = node.rect.x - 7.9;
                r.y = prevY;
                r.w = 220;
                r.h = itemH;
                SDL_RenderFillRect(renderer, &r);
                SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);

                SDL_Surface *s1 = TTF_RenderText_Blended(font2, item, (SDL_Color){180, 180, 180, 255});
                SDL_Texture *t1 = SDL_CreateTextureFromSurface(renderer, s1);
                r.x = r.x + 10;
                r.y = r.y + 5;
                r.h = s1->h;
                r.w = (s1->w + 0.0) / s1->h * r.h;

                SDL_RenderCopy(renderer, t1, NULL, &r);
                SDL_DestroyTexture(t1);
                SDL_FreeSurface(s1);

                j++;
                item = node.list[j];
                prevY += itemH;
            }

            // SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_Rect r;
            r.x = node.rect.x - 7.9;
            r.y = node.rect.y + node.rect.h;
            r.w = 220;
            r.h = itemH * j;

            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderDrawRect(renderer, &r);
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
    SDL_RenderDrawLine(renderer, 0, TOPNAV_H, 0, WINDOW_H); // Drawing MENUBAR Border
    if (showMenu)
        SDL_RenderDrawLine(renderer, MENU_W, TOPNAV_H, MENU_W, WINDOW_H); // Drawing MENU Border
    SDL_RenderDrawLine(renderer, 0, TOPNAV_H, WINDOW_W, TOPNAV_H);        // Drawing TOPNAV Border

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

    SDL_Rect r = {
        0,
        TOPNAV_H,
        MENU_W,
        LEFT_MENU[0].rect.h + 30};

    SDL_SetRenderDrawColor(renderer,  23, 23, 23, 255);
    SDL_RenderFillRect(renderer, &r);

    int i;
    // Menu Bar Buttons
    for (i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
    {
        MENU_BAR_NODE node = LEFT_MENU[i];
        if (menu_state == i)
        {
            SDL_Rect r = {
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
            SDL_RenderCopy(renderer, node.active_texture, NULL, &node.rect);
        }
        else
        {
            SDL_RenderCopy(renderer, node.texture, NULL, &node.rect);
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
    SDL_RenderDrawLine(renderer, FILEBAR_bg_rect.x, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h, WINDOW_W, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h);
    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);

    FileBarItem *node = FileBar;
    int i = 0, x = FILEBAR_bg_rect.x;

    while (node != NULL)
    {
        SDL_Color color = {239, 239, 239, 255};
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->name, color);
        if (!node->t1)
        {
            node->t1 = SDL_CreateTextureFromSurface(renderer, s1);
        }

        node->r1.x = x + FILEMENU_SCROLL_X;
        node->r1.y = FILEBAR_bg_rect.y;
        node->r1.w = s1->w + 40;
        node->r1.h = FILEBAR_bg_rect.h;

        node->r2.x = x + node->r1.w / 2 - s1->w / 2 + FILEMENU_SCROLL_X;
        node->r2.y = FILEBAR_bg_rect.y + FILEBAR_bg_rect.h / 2 - s1->h / 2;
        node->r2.w = s1->w;
        node->r2.h = s1->h;

        x += node->r1.w;

        if (node == currentActiveTag)
        {
            SDL_SetRenderDrawColor(renderer, 66, 66, 66, 100);
            SDL_RenderFillRect(renderer, &node->r1);
        }

        SDL_SetRenderDrawColor(renderer, 56, 56, 56, 100);
        SDL_RenderDrawLine(renderer, node->r1.x + node->r1.w, FILEBAR_bg_rect.y, node->r1.x + node->r1.w, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h);

        SDL_RenderCopy(renderer, node->t1, NULL, &node->r2);

        SDL_FreeSurface(s1);
        node = node->next;
        i += 1;
    }
}

void renderTextEditor()
{

    FileBarItem *node = currentActiveTag;

    if (!node)
    {
        // SDL_RenderCopy(renderer, logoTexture2, NULL, &logoRect2);
        return;
    }

    FileLine *line = node->lines;
    if (!line)
        return;

    int y = 0;

    SDL_Rect minimapBgRect = {
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

        if (!word)
        {
            Token *t = createToken("", 1, (SDL_Color){200, 255, 200, 255});
            t->next = NULL;
            t->prev = NULL;
            line->word = t;
            word = t;
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
            line = line->next;
            continue;
        }

        while (word)
        {
            word->len = strlen(word->content);
            int w = 0, h = 0;
            SDL_QueryTexture(word->t1, NULL, NULL, &w, &h);
            h = EDITOR_FONT_HEIGHT;
            SDL_Rect r1 = {
                LINE_NUMBER_WIDTH + FILEBAR_bg_rect.x + 4 + currentActiveTag->EDITOR_SCROLL_X + x,
                FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * h,
                w,
                h};

            /* ===== Draw Cursor ===== */
            if (word == currentActiveTag->currentWord)
            {
                int x = r1.x + cursor->w * currentActiveTag->startIndex;

                SDL_Rect cursorRect = {x, r1.y, 2, cursor->h};

                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderFillRect(renderer, &cursorRect);
            }
            if (r1.x + r1.w > MENU_W && r1.x < WINDOW_W)
            {
                SDL_RenderCopy(renderer, word->t1, NULL, &r1);
            }
            // printf("[%s]", word->content);
            x += w;
            word = word->next;
        }

        if (hasSelectionStarted && currentActiveTag->SELECTION_START_WORD)
        {
            SDL_Rect selectionRect = {
                MENU_W + LINE_NUMBER_WIDTH,
                FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * EDITOR_FONT_HEIGHT,
                1000,
                EDITOR_FONT_HEIGHT};

            SDL_SetRenderDrawColor(renderer, 100, 150, 250, 50);
            SDL_RenderFillRect(renderer, &selectionRect);
        }

        // printf("\n");
        /*===== Draw Line Number =====*/
        char text[12];
        sprintf(text, "%d", y + 1);
        SDL_Surface *lineNoSurface = TTF_RenderText_Blended(jetbrains_regular, text, (SDL_Color){155, 155, 155, 255});
        SDL_Texture *lineNoTexture = SDL_CreateTextureFromSurface(renderer, lineNoSurface);

        SDL_Rect lineNoRect = {
            FILEBAR_bg_rect.x + LINE_NUMBER_WIDTH / 2 - lineNoSurface->w / 2,
            FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * lineNoSurface->h,
            lineNoSurface->w,
            lineNoSurface->h};

        SDL_Rect lineNoBgRect = {
            MENU_W,
            FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + 4 + (y + currentActiveTag->EDITOR_SCROLL_Y / EDITOR_FONT_SIZE) * lineNoSurface->h,
            LINE_NUMBER_WIDTH,
            lineNoSurface->h};

        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
        SDL_RenderFillRect(renderer, &lineNoBgRect);
        SDL_RenderCopy(renderer, lineNoTexture, NULL, &lineNoRect);

        SDL_FreeSurface(lineNoSurface);
        SDL_DestroyTexture(lineNoTexture);

        y++;
        line = line->next;
    }

    /* ===== Render Minimap =====*/
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 250);
    SDL_RenderFillRect(renderer, &minimapBgRect);

    line = node->lines;
    y = 0;
    while (line)
    {

        Token *wrd = line->word;
        int i = 0;
        while (wrd)
        {
            if (strcmp(wrd->content, " ") == 0)
            {
                i++;
                wrd = wrd->next;
                continue;
            }
            SDL_Rect minimapRect = {
                WINDOW_W - 100 + 4 + i * 1,
                FILEBAR_bg_rect.y + FILEBAR_bg_rect.h + y * 2,
                (int)strlen(wrd->content) * 1,
                1.5};
            if (wrd->color.r >= 200 && wrd->color.g >= 200 && wrd->color.b >= 200)
                SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
            else
                SDL_SetRenderDrawColor(renderer, wrd->color.r, wrd->color.g, wrd->color.b, wrd->color.a);
            SDL_RenderFillRect(renderer, &minimapRect);
            i += (int)strlen(wrd->content);
            wrd = wrd->next;
        }

        y++;
        line = line->next;
    }

    /* ===== Draw Highlight Rect ===== */
    SDL_Rect highlightRect = {
        WINDOW_W - 100,
        FILEBAR_bg_rect.y + FILEBAR_bg_rect.h - (0.0 + currentActiveTag->EDITOR_SCROLL_Y) / cursor->h * 2,
        100,
        70};

    SDL_SetRenderDrawColor(renderer, 137, 137, 137, 100);
    SDL_RenderFillRect(renderer, &highlightRect);

    /*===== Draw Borders =====*/
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderDrawLine(renderer, FILEBAR_bg_rect.x + LINE_NUMBER_WIDTH, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h, FILEBAR_bg_rect.x + LINE_NUMBER_WIDTH, WINDOW_H);

    SDL_SetRenderDrawColor(renderer, 57, 57, 57, 255);
    SDL_RenderDrawLine(renderer, WINDOW_W - 100, FILEBAR_bg_rect.y + FILEBAR_bg_rect.h, WINDOW_W - 100, WINDOW_H);
}

void renderExplorer()
{

    MENU_BAR_NODE node = LEFT_MENU[0];

    SDL_Color color = {180, 180, 180, 255};

    // Initialize Explorer if not
    // Textures
    if (!Explorer.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, "You have not opened", color);
        Explorer.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    if (!Explorer.t2)
    {
        SDL_Surface *s2 = TTF_RenderText_Blended(poppins_regular, "any folder yet", color);
        Explorer.t2 = SDL_CreateTextureFromSurface(renderer, s2);
    }

    if (!Explorer.t3)
    {
        SDL_Color color = {233, 233, 233, 255};
        SDL_Surface *s3 = TTF_RenderText_Blended(font2, "Open Folder", color);
        Explorer.t3 = SDL_CreateTextureFromSurface(renderer, s3);
    }

    // if(!Explorer.t4){
    //     SDL_Surface* s4 = TTF_RenderText_Blended(poppins_regular, "Open Recent", color);
    //     Explorer.t4 = SDL_CreateTextureFromSurface(renderer, s4);
    // }

    // Rects
    if (!Explorer.r1.x)
    {
        int w, h;
        SDL_QueryTexture(Explorer.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h};
        Explorer.r1 = r1; // First Text Box
    }

    if (!Explorer.r2.x)
    {
        int w, h;
        SDL_QueryTexture(Explorer.t2, NULL, NULL, &w, &h);
        SDL_Rect r2 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + Explorer.r1.h,
            w,
            h};
        Explorer.r2 = r2; // Second Text Box
    }

    if (!Explorer.r3.x)
    {
        int w, h;
        SDL_QueryTexture(Explorer.t1, NULL, NULL, &w, &h);
        SDL_Rect r3 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3 * MENU_PAD_Y + Explorer.r1.h + Explorer.r2.h - 2.5,
            MENU_W - 2 * MENU_PAD_Y,
            h + 5};
        Explorer.r3 = r3; // First Button Container
    }

    if (!Explorer.r4.x)
    {
        int w, h;
        SDL_QueryTexture(Explorer.t3, NULL, NULL, &w, &h);
        SDL_Rect r4 = {
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
        SDL_RenderCopy(renderer, Explorer.t1, NULL, &Explorer.r1);
        SDL_RenderCopy(renderer, Explorer.t2, NULL, &Explorer.r2);
        SDL_RenderCopy(renderer, Explorer.t3, NULL, &Explorer.r4);
    }
    else
    {
        renderFolder(&Folder, &i, 0);
    }

    // Display the Menu Name
    if (!Folder)
    {
        SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);
    }
    else
    {
        if (!Folder->t1)
        {
            SDL_Color color = {155, 155, 155, 255};
            SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, Folder->name, color);
            Folder->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_FreeSurface(s1);
        }

        SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
        SDL_Rect BG = {
            1,
            TOPNAV_H + MENU_BAR_H,
            MENU_W - 1,
            Folder->r1.h * 2};
        SDL_RenderFillRect(renderer, &BG);
        // SDL_SetRenderDrawColor(renderer, 56, 56, 56, 255);
        // SDL_RenderDrawLine(renderer, BG.x, BG.y, BG.x + BG.w, BG.y);
        SDL_RenderCopy(renderer, Folder->t1, NULL, &Folder->r1);
    }
}

void renderFolder(FileNode **folder, int *i, int padX)
{

    if (!(*folder)->child)
    {
        if (!(*folder)->isDirOpened)
        {

            DIR *dir = opendir((*folder)->path);
            if (!dir)
            {
                perror("opendir");
                return;
            }

            struct dirent *entry;
            char fullpath[2048];
            struct stat st;

            FileNode *head = NULL;
            while ((entry = readdir(dir)) != NULL)
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                // printf("%s,", entry->d_name);
                int type = 0;
                snprintf(fullpath, sizeof(fullpath), "%s/%s", (*folder)->path, entry->d_name);

                if (stat(fullpath, &st) == 0)
                {
                    if (S_ISDIR(st.st_mode))
                    {
                        type = 1;
                    }
                    else if (S_ISREG(st.st_mode))
                    {
                        type = 0;
                    }
                    else
                    {
                        type = 0;
                    }
                }
                else
                {
                    perror("stat");
                }

                FileNode *node = createFileNode(entry->d_name, fullpath, type);

                node->opened = 0;
                node->next = head;
                if (head)
                    head->prev = node;

                head = node;
            }

            (*folder)->child = head;
            (*folder)->isDirOpened = 1;
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

        if (!node->t1)
        {
            // SDL_Color color = {239, 239, 239, 255};
            SDL_Color color = {200, 200, 200, 255};
            SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->name, color);
            node->t1 = SDL_CreateTextureFromSurface(renderer, s1);
            SDL_FreeSurface(s1);
        }

        SDL_Color color = {239, 239, 239, 255};
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, node->name, color);
        node->r1.x = padX * 10 + LEFT_MENU[0].rect.x + s1->h + 2;
        node->r1.w = s1->w;
        node->r1.y = MENU_BAR_H + LEFT_MENU[0].rect.y + LEFT_MENU[0].rect.h + s1->h * (*i) + MENU_PAD_Y / 2 * (*i) + EXPLORER_SCROLL_Y;
        node->r1.h = s1->h;
        SDL_FreeSurface(s1);

        if (node->hovered)
        {
            SDL_Rect hoverRect = {
                MENU_PAD_X,
                node->r1.y,
                MENU_W - 2 * MENU_PAD_X,
                node->r1.h};

            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
            SDL_RenderFillRect(renderer, &hoverRect);
        }

        FileIcons.r1.x = node->r1.x - 2 - node->r1.h;
        FileIcons.r1.y = node->r1.y + 2;
        FileIcons.r1.w = node->r1.h - 4;
        FileIcons.r1.h = node->r1.h - 4;

        if (node->isDir)
        {
            if (node->opened)
            {
                SDL_RenderCopy(renderer, FileIcons.t3, NULL, &FileIcons.r1); // Display Folder Open Icon
                (*i) += 1;
                renderFolder(&node, i, padX + 1);
                SDL_RenderCopy(renderer, node->t1, NULL, &node->r1);
                node = node->next;
                continue;
            }
            else
                SDL_RenderCopy(renderer, FileIcons.t2, NULL, &FileIcons.r1); // Display Folder Closed Icon
        }
        else
        {
            // Display File Icons
            SDL_Texture *fileTexture = FileIcons.t1;
            if (node->type == CLANG)
                fileTexture = FileIcons.t4;
            else if (node->type == CHEADER)
                fileTexture = FileIcons.t4;
            else if (node->type == HTML)
                fileTexture = FileIcons.t5;
            else if (node->type == CSS)
                fileTexture = FileIcons2.t1;
            else if (node->type == IMG)
                fileTexture = FileIcons2.t2;

            SDL_RenderCopy(renderer, fileTexture, NULL, &FileIcons.r1);
        }

        SDL_RenderCopy(renderer, node->t1, NULL, &node->r1); // Display File/Folder name
        node = node->next;
        (*i) += 1;
    }
}

void renderSearch()
{
    MENU_BAR_NODE node = LEFT_MENU[1];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {100, 100, 100, 255};

    // Textures
    if (!Search.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, SearchMenu.txt1, color);
        Search.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    if (!Search.t2)
    {
        SDL_Surface *s2 = TTF_RenderText_Blended(poppins_regular, SearchMenu.txt2, color);
        Search.t2 = SDL_CreateTextureFromSurface(renderer, s2);
    }

    // Rects

    if (!Search.r1.x)
    {
        int w, h;
        SDL_QueryTexture(Search.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            MENU_W - 2 * MENU_PAD_X,
            h + 5};
        Search.r1 = r1;
    }

    if (!Search.r2.x)
    {
        int w, h;
        SDL_QueryTexture(Search.t1, NULL, NULL, &w, &h);
        SDL_Rect r2 = {
            MENU_PAD_X + 10,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + 2.5,
            w,
            h};
        Search.r2 = r2;
    }

    if (!Search.r3.x)
    {
        int w, h;
        SDL_QueryTexture(Search.t2, NULL, NULL, &w, &h);
        SDL_Rect r3 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3.6 / 2 * MENU_PAD_Y + Search.r1.h,
            MENU_W - 2 * MENU_PAD_X,
            h + 5};
        Search.r3 = r3;
    }

    if (!Search.r4.x)
    {
        int w, h;
        SDL_QueryTexture(Search.t2, NULL, NULL, &w, &h);
        SDL_Rect r4 = {
            MENU_PAD_X + 10,
            node.text_rect.y + node.text_rect.h + 3.6 / 2 * MENU_PAD_Y + 2.5 + Search.r1.h,
            w,
            h};
        Search.r4 = r4;
    }

    SDL_SetRenderDrawColor(renderer, 6, 6, 6, 255);
    SDL_RenderFillRect(renderer, &Search.r1);
    SDL_RenderFillRect(renderer, &Search.r3);
    SDL_RenderCopy(renderer, Search.t1, NULL, &Search.r2);
    SDL_RenderCopy(renderer, Search.t2, NULL, &Search.r4);
}

void renderGithub()
{
    MENU_BAR_NODE node = LEFT_MENU[2];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {180, 180, 180, 255};

    // Textures
    if (!Github.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, "Import From a GitHub", color);
        Github.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    if (!Github.t2)
    {
        SDL_Surface *s2 = TTF_RenderText_Blended(poppins_regular, "Repository", color);
        Github.t2 = SDL_CreateTextureFromSurface(renderer, s2);
    }

    if (!Github.t3)
    {
        SDL_Color color = {233, 233, 233, 255};
        SDL_Surface *s3 = TTF_RenderText_Blended(font2, "Connect Github", color);
        Github.t3 = SDL_CreateTextureFromSurface(renderer, s3);
    }

    // if(!Github.t4){
    //     SDL_Surface* s4 = TTF_RenderText_Blended(poppins_regular, "Open Recent", color);
    //     Github.t4 = SDL_CreateTextureFromSurface(renderer, s4);
    // }

    // Rects
    if (!Github.r1.x)
    {
        int w, h;
        SDL_QueryTexture(Github.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h};
        Github.r1 = r1; // First Text Box
    }

    if (!Github.r2.x)
    {
        int w, h;
        SDL_QueryTexture(Github.t2, NULL, NULL, &w, &h);
        SDL_Rect r2 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y + Github.r1.h,
            w,
            h};
        Github.r2 = r2; // Second Text Box
    }

    if (!Github.r3.x)
    {
        int w, h;
        SDL_QueryTexture(Github.t1, NULL, NULL, &w, &h);
        SDL_Rect r3 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + 3 * MENU_PAD_Y + Github.r1.h + Github.r2.h - 2.5,
            MENU_W - 2 * MENU_PAD_Y,
            h + 5};
        Github.r3 = r3; // First Button Container
    }

    if (!Github.r4.x)
    {
        int w, h;
        SDL_QueryTexture(Github.t3, NULL, NULL, &w, &h);
        SDL_Rect r4 = {
            MENU_W / 2 - w / 2,
            node.text_rect.y + node.text_rect.h + 3 * MENU_PAD_Y + Github.r1.h + Github.r2.h,
            w,
            h};
        Github.r4 = r4; // First Button Text Container
    }

    SDL_SetRenderDrawColor(renderer, 106, 90, 205, 255);
    SDL_RenderFillRect(renderer, &Github.r3);
    SDL_RenderCopy(renderer, Github.t1, NULL, &Github.r1);
    SDL_RenderCopy(renderer, Github.t2, NULL, &Github.r2);
    SDL_RenderCopy(renderer, Github.t3, NULL, &Github.r4);
}

void renderExtentions()
{
    MENU_BAR_NODE node = LEFT_MENU[3];
    SDL_RenderCopy(renderer, node.text_texture, NULL, &node.text_rect);

    SDL_Color color = {130, 130, 130, 255};

    // Textures
    if (!Extentions.t1)
    {
        SDL_Surface *s1 = TTF_RenderText_Blended(poppins_regular, "Feature Coming Soon", color);
        Extentions.t1 = SDL_CreateTextureFromSurface(renderer, s1);
    }

    // Rects
    if (!Extentions.r1.x)
    {
        int w, h;
        SDL_QueryTexture(Extentions.t1, NULL, NULL, &w, &h);
        SDL_Rect r1 = {
            MENU_PAD_X,
            node.text_rect.y + node.text_rect.h + MENU_PAD_Y,
            w,
            h};
        Extentions.r1 = r1; // First Text Box
    }

    SDL_RenderCopy(renderer, Extentions.t1, NULL, &Extentions.r1);
}

void renderFooter()
{
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
    SDL_Rect r1 = {
        0,
        WINDOW_H - FOOTER_H,
        WINDOW_W,
        FOOTER_H};
    SDL_RenderFillRect(renderer, &r1);
    SDL_SetRenderDrawColor(renderer, 47, 47, 47, 255);
    SDL_RenderDrawLine(renderer, r1.x, r1.y, r1.x + r1.w, r1.y);
}