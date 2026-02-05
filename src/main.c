/**
 * @file main.c
 * @brief Main application entry point and core event loop
 *
 * This file contains the main application logic, including:
 * - SDL initialization and window creation
 * - Main event loop for handling user input
 * - Core UI rendering and state management
 * - File explorer and editor coordination
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/renderer.h"
#include "files.h"
#include "events.h"
#include "parser.h"

// Function prototypes
void renderFile();
char *open_folder_dialog();
void *open_folder_thread();

// ======== GLOBAL STATE VARIABLES ========

/** @brief Current editor state (0 = normal, 1 = loading, etc.) */
int editorState = 0;

/**
 * @brief Main application entry point
 *
 * Initializes SDL, creates the main window, sets up the UI components,
 * and enters the main event loop for handling user interactions.
 *
 * @return 0 on successful exit, non-zero on error
 */
int main()
{
    // Initialize SDL2, fonts, and all UI components
    init();

    int i = 0;

    // ======== LOADING SCREEN SETUP ========

    SDL_FRect loadingBgRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
    SDL_RenderFillRect(renderer, &loadingBgRect);

    // SDL_FRect loadingMenuRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderFillRect(renderer, &MENU_bg_rect);

    // SDL_FRect loadingBgRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 27, 27, 27, 255);
    SDL_RenderFillRect(renderer, &TOPNAV_bg_rect);

    SDL_FRect loadingFooterRect = {0, WINDOW_H - FOOTER_H, WINDOW_W, FOOTER_H};
    SDL_SetRenderDrawColor(renderer, 57, 57, 57, 255);
    SDL_RenderFillRect(renderer, &loadingFooterRect);

    SDL_Surface *bgImageSurface = IMG_Load("assets/nature.png");
    // SDL_Surface *bgImageSurface = IMG_Load("assets/landscape2.jpg");
    if (!bgImageSurface)
    {
        fprintf(stderr, "Error loading background image: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *bgImage = SDL_CreateTextureFromSurface(renderer, bgImageSurface);
    SDL_SetTextureScaleMode(bgImage, SDL_SCALEMODE_LINEAR);

    if (!bgImage)
    {
        fprintf(stderr, "Error creating background texture: %s\n", SDL_GetError());
        SDL_DestroySurface(bgImageSurface);
        return 1;
    }

    SDL_FRect bgRect = {
        MENU_W,
        0,
        (bgImageSurface->w + 0.0) / bgImageSurface->h * WINDOW_H,
        WINDOW_H};

    SDL_FRect fullScreenRect = {
        0,
        0,
        WINDOW_W,
        WINDOW_H};

    /* ===== Props ===== */
    SDL_Surface *propS1 = IMG_Load("assets/prop1.png");
    SDL_Texture *prop1 = SDL_CreateTextureFromSurface(renderer, propS1);
    SDL_SetTextureScaleMode(prop1, SDL_SCALEMODE_NEAREST);

    SDL_FRect propRect1 = {
        100, 0, 100, 100};
    SDL_FPoint propCenter1 = {(propRect1.x + propRect1.w) / 2, (propRect1.y + propRect1.h) / 2};
    int propangle1 = 0;

    // Load the I-beam (text) cursor
    SDL_Cursor *ibeam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
    // SDL_SetCursor(ibeam);

    // Load the Pointer cursor
    SDL_Cursor *arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    // SDL_SetCursor(ibeam);

    int running = 1;
    int time = 0;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                int x = (int)event.button.x;
                int y = (int)event.button.y;

                MOUSE_X = x;
                MOUSE_Y = y;
                IS_MOUSE_DOWN = 1;

                // Menu Buttons
                if (x > 0 && x < MENU_W)
                {

                    // Open Folder button in Exploror
                    if (!IS_TOPNAV_MENU_DOWN && !Folder && menu_state == 0 && Explorer.t3 && x > Explorer.r3.x && x < Explorer.r3.x + Explorer.r3.w && y > Explorer.r3.y && y < Explorer.r3.y + Explorer.r3.h)
                    {
                        printf("Opening folder dialog...\n");
                        printf("%d\n", IS_TOPNAV_MENU_DOWN);
                        pthread_t folder_thread;
                        pthread_create(&folder_thread, NULL, open_folder_thread, NULL);
                        pthread_detach(folder_thread); // optional: auto-clean thread
                    }

                    // Files and Folders in Exploror
                    if (!IS_TOPNAV_MENU_DOWN && Folder)
                    {

                        handleExplorerItemsClick(&Folder, x, y);
                    }
                }

                // Top Nav Bar Buttons

                IS_TOPNAV_MENU_DOWN = 0;

                for (i = 0; i < (int)(sizeof(TOPNAV_MENU) / sizeof(TOPNAV_MENU[0])); i++)
                {
                    TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];
                    SDL_FRect r;
                    r.x = node->rect.x - 7.9;
                    r.w = node->rect.w + 16.8;
                    r.y = node->rect.y;
                    r.h = node->rect.h;
                    if (x > r.x && x < r.x + r.w && y > r.y && y < r.y + r.h)
                    {
                        if (node->clicked)
                            node->clicked = 0;
                        else
                            node->clicked = 1;
                        IS_TOPNAV_MENU_DOWN = 1;
                    }
                    else
                    {
                        node->clicked = 0;
                    }
                }

                // Menu Bar Buttons
                if (x < MENU_W)
                {
                    for (i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
                    {
                        MENU_BAR_NODE *node = &LEFT_MENU[i];

                        SDL_FRect r;
                        r.x = node->rect.x;
                        r.w = node->rect.w;
                        r.y = node->rect.y - 10;
                        r.h = node->rect.h + 20;

                        if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                        {
                            if (menu_state == i)
                            {
                                if (showMenu == 0)
                                {
                                    showMenu = 1;
                                    logoRect2.x = MENU_W + (WINDOW_W - MENU_W) / 2 - dlw2 / 2;
                                    FILEBAR_bg_rect.x = MENU_W + 1;
                                }
                                else
                                {
                                    showMenu = 0;
                                    logoRect2.x = (WINDOW_W) / 2 - dlw2 / 2;
                                    FILEBAR_bg_rect.x = 1;
                                }
                            }
                            else
                            {
                                showMenu = 1;
                                FILEBAR_bg_rect.x = MENU_W;
                                logoRect2.x = MENU_W + (WINDOW_W - MENU_W) / 2 - dlw2 / 2;
                            }
                            menu_state = i;
                        }
                    }
                }

                // TOPNAV_RIGHT Buttons
                if (y < TOPNAV_H + 50)
                {
                    for (i = 0; i < (int)(sizeof(TOPNAV_RIGHT) / sizeof(TOPNAV_RIGHT[0])); i++)
                    {
                        MENU_BAR_NODE *node = &TOPNAV_RIGHT[i];

                        SDL_FRect r;
                        r.x = node->rect.x;
                        r.w = node->rect.w;
                        r.y = node->rect.y - 10;
                        r.h = node->rect.h + 20;

                        if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                        {
                            node->clicked = !node->clicked;
                            if (i == 0)
                            {
                                if (node->clicked)
                                    RIGHTPANEL_X = WINDOW_W - RIGHTPANEL_W;
                                else
                                    RIGHTPANEL_X = WINDOW_W;
                            }
                        }
                        else if (!node->clicked)
                        {
                            node->clicked = 0;
                        }
                    }
                }

                // File Bar
                if (FileBar && x > MENU_W && x < WINDOW_W && y > TOPNAV_H && y < 2 * TOPNAV_H)
                {
                    FileBarItem *node = FileBar;

                    while (node)
                    {
                        int x1 = node->r1.x;
                        int x2 = x1 + node->r1.w;
                        int y1 = node->r1.y;
                        int y2 = y1 + node->r1.h;
                        if (x > x1 && x < x2 && y > y1 && y < y2)
                        {
                            node->active = 1;
                            currentActiveTag = node;
                        }
                        else
                        {
                            node->active = 0;
                        }
                        node = node->next;
                    }
                }

                // Text Editor
                if (currentActiveTag && x > MENU_W + 70 && x < WINDOW_W - MINIMAP_W && y > 2 * TOPNAV_H && y < WINDOW_H - FOOTER_H)
                {
                    IS_SELECTING = 1;
                    int scrollX = currentActiveTag->EDITOR_SCROLL_X;
                    // int scrollY = currentActiveTag->EDITOR_SCROLL_Y;

                    // int ty = 0;
                    int cy = y - 2 * TOPNAV_H;

                    // if (scrollY < -EDITOR_FONT_HEIGHT)
                    //     currentActiveTag->visibleLine = currentActiveTag->lines;

                    FileLine *node = currentActiveTag->visibleLine;
                    if (node)
                    {
                        int offset = cy / EDITOR_FONT_HEIGHT;
                        // printf("%d\n", offset);
                        while (offset > 0 && node->next)
                        {
                            node = node->next;
                            offset--;
                        }

                        currentActiveTag->currentLine = node;

                        Token *word = node->word;
                        int sum = scrollX + cursor->w;

                        while (sum < x - MENU_W - 70 && word->next)
                        {
                            if (sum + word->len * cursor->w >= x - MENU_W - 70)
                            {
                                break;
                            }
                            sum += word->len * cursor->w;
                            word = word->next;
                        }
                        // word = word;
                        // sum += word->len * cursor->w;

                        currentActiveTag->currentWord = word;
                        currentActiveTag->startIndex = (x - (sum + MENU_W + 66)) / cursor->w;

                        if (currentActiveTag->startIndex > word->len)
                            currentActiveTag->startIndex = word->len;
                        if (currentActiveTag->startIndex < 0)
                            currentActiveTag->startIndex = 0;

                        currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                        currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                        currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                        // printf("%s %s %d\n", currentActiveTag->currentLine->word->content, currentActiveTag->currentWord->content, currentActiveTag->startIndex);
                    }
                    else
                    {
                        // printf("Node Does not Exist\n");
                    }
                }

                // Completion Box
                if (currentActiveTag)
                {
                    showCompletion = 0;
                }
            }

            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                // if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                // {
                WINDOW_W = event.window.data1;
                WINDOW_H = event.window.data2;

                MENU_W = WINDOW_W / 6;
                FILEBAR_bg_rect.w = MENU_W;
                FILEBAR_bg_rect.h = WINDOW_H;

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

                // BG Rect
                bgRect.x = MENU_W;
                bgRect.w = (bgImageSurface->w + 0.0) / bgImageSurface->h * WINDOW_H;
                bgRect.h = WINDOW_H;

                // Full Screen Rect
                fullScreenRect.w = WINDOW_W;
                fullScreenRect.h = WINDOW_H;

                // TOPNAV_RIGHT BUTTONS

                int prevX = 0;
                for (int i = 0; i < (int)(sizeof(TOPNAV_RIGHT) / sizeof(TOPNAV_RIGHT[0])); i++)
                    TOPNAV_RIGHT[0].rect.x = WINDOW_W - TOPNAV_RIGHT[0].rect.w - TOPNAV_H / 2 - (prevX++);

                // Explorer Menu
                Explorer.r1.x = 0;
                Explorer.r2.x = 0;
                Explorer.r3.x = 0;
                Explorer.r4.x = 0;

                // Search Menu
                Search.r1.x = 0;
                Search.r2.x = 0;
                Search.r3.x = 0;
                Search.r4.x = 0;

                // Github Menu
                Github.r1.x = 0;
                Github.r2.x = 0;
                Github.r3.x = 0;
                Github.r4.x = 0;

                // Github Menu
                Footer.r1.x = 0;
                Footer.r2.x = 0;
                Footer.r3.x = 0;
                Footer.r4.x = 0;
                // }
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                IS_MOUSE_DOWN = 0;
                IS_SELECTING = 0;
            }

            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {

                int x = (int)event.motion.x;
                int y = (int)event.motion.y;

                MOUSE_X = x;
                MOUSE_Y = y;

                // Top Nav Left Buttons
                if (y < TOPNAV_H + 50)
                {
                    for (i = 0; i < 5; i++)
                    {
                        TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];

                        SDL_FRect r;
                        r.x = node->rect.x - 7.9;
                        r.w = node->rect.w + 16.8;
                        r.y = node->rect.y;
                        r.h = node->rect.h;

                        if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                        {
                            node->isActive = 1;
                        }
                        else if (!node->clicked)
                        {
                            node->isActive = 0;
                        }
                    }
                }

                // TOPNAV_RIGHT Buttons
                if (y < TOPNAV_H + 50)
                {
                    for (i = 0; i < (int)(sizeof(TOPNAV_RIGHT) / sizeof(TOPNAV_RIGHT[0])); i++)
                    {
                        MENU_BAR_NODE *node = &TOPNAV_RIGHT[i];

                        SDL_FRect r;
                        r.x = node->rect.x;
                        r.w = node->rect.w;
                        r.y = node->rect.y - 10;
                        r.h = node->rect.h + 20;

                        if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                        {
                            node->isActive = 1;
                        }
                        else
                        {
                            node->isActive = 0;
                        }
                    }
                }

                // Menu Bar Buttons
                if (x < MENU_W)
                {
                    for (i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
                    {
                        MENU_BAR_NODE *node = &LEFT_MENU[i];

                        SDL_FRect r;
                        r.x = node->rect.x;
                        r.w = node->rect.w;
                        r.y = node->rect.y - 10;
                        r.h = node->rect.h + 20;

                        if (x > r.x && x < r.w + r.x && y > r.y && y < r.y + r.h)
                        {
                            node->isActive = 1;
                        }
                        else if (!node->clicked)
                        {
                            node->isActive = 0;
                        }
                    }
                }

                // Files and Folders in Exploror
                if (x > 0 && x < MENU_W + 150)
                {
                    if (Folder)
                    {

                        handleExplorerItemsHover(&Folder, x, y);
                    }
                }

                // Text Editor
                if (currentActiveTag && currentActiveTag->type != AETHER_IMG && x > MENU_W && x < WINDOW_W - MINIMAP_W && y > 2 * TOPNAV_H && y < WINDOW_H - FOOTER_H)
                {
                    SDL_SetCursor(ibeam);
                }
                else
                {
                    SDL_SetCursor(arrowCursor);
                }

                // Handle Selection
                if (IS_SELECTING)
                {
                    currentActiveTag->SELECTION_START_INDEX = (x - MENU_W - 70) / cursor->w;

                    int scrollX = currentActiveTag->EDITOR_SCROLL_X;
                    // int scrollY = currentActiveTag->EDITOR_SCROLL_Y;

                    // int ty = 0;
                    int cy = y - 2 * TOPNAV_H;

                    // if (scrollY < -EDITOR_FONT_HEIGHT)
                    //     currentActiveTag->visibleLine = currentActiveTag->lines;

                    FileLine *node = currentActiveTag->visibleLine;
                    if (node)
                    {
                        int offset = cy / EDITOR_FONT_HEIGHT;
                        // printf("%d\n", offset);
                        while (offset > 0 && node->next)
                        {
                            node = node->next;
                            offset--;
                        }

                        currentActiveTag->currentLine = node;

                        Token *word = node->word;
                        int sum = scrollX + cursor->w;

                        while (sum < x - MENU_W - 70 && word->next)
                        {
                            if (sum + word->len * cursor->w >= x - MENU_W - 70)
                            {
                                break;
                            }
                            sum += word->len * cursor->w;
                            word = word->next;
                        }
                        // word = word;
                        // sum += word->len * cursor->w;

                        currentActiveTag->currentWord = word;
                        currentActiveTag->startIndex = (x - (sum + MENU_W + 70)) / cursor->w;

                        if (currentActiveTag->startIndex > word->len)
                            currentActiveTag->startIndex = word->len;
                        if (currentActiveTag->startIndex < 0)
                            currentActiveTag->startIndex = 0;
                        // printf("%s %s %d\n", currentActiveTag->currentLine->word->content, currentActiveTag->currentWord->content, currentActiveTag->startIndex);
                    }
                    else
                    {
                        // printf("Node Does not Exist\n");
                    }
                }
            }

            if (event.type == SDL_EVENT_MOUSE_WHEEL)
            {
                if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
                {
                    handleMouseScroll(-event.wheel.x, -event.wheel.y);
                }
                else
                {
                    handleMouseScroll(event.wheel.x, event.wheel.y);
                }
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                SDL_Keycode key = event.key.key;
                SDL_Keymod mod = event.key.mod;

                if (currentActiveTag)
                {
                    if (key == SDLK_BACKSPACE)
                    {
                        leftDeleteChar();
                        currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                        currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                        currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                        showCompletion = 0;
                    }

                    if (key == SDLK_UP)
                    {
                        if (CompletionBox.active && showCompletion)
                        {
                            if (CompletionBox.active->prev)
                                CompletionBox.active = CompletionBox.active->prev;
                            else
                                CompletionBox.active = CompletionBox.tail;
                        }
                        else
                        {
                            moveCursorUp();

                            currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                            currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                            currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                            showCompletion = 0;
                        }
                    }
                    if (key == SDLK_DOWN)
                    {
                        if (showCompletion && CompletionBox.active)
                        {
                            if (CompletionBox.active->next)
                                CompletionBox.active = CompletionBox.active->next;
                            else
                                CompletionBox.active = CompletionBox.list;
                        }
                        else
                        {
                            moveCursorDown();

                            currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                            currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                            currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                            showCompletion = 0;
                        }
                    }
                    if (key == SDLK_LEFT)
                    {
                        moveCursorLeft();

                        currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                        currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                        currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                        if (showCompletion)
                            getCompletion(currentActiveTag->currentWord->content, currentActiveTag->startIndex);
                    }

                    if (key == SDLK_RIGHT)
                    {
                        moveCursorRight();

                        currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                        currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                        currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                        if (showCompletion)
                            getCompletion(currentActiveTag->currentWord->content, currentActiveTag->startIndex);
                    }
                    if (key == SDLK_RETURN)
                    {
                        if (showCompletion)
                        {
                            replaceWord(CompletionBox.active->text);
                        }
                        else
                        {
                            createNewline();
                        }

                        currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                        currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                        currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                        showCompletion = 0;
                    }

                    if ((mod & SDL_KMOD_CTRL) && key == SDLK_TAB)
                    {
                        if (mod & SDL_KMOD_SHIFT)
                        {
                            if (currentActiveTag->prev)
                                currentActiveTag = currentActiveTag->prev;
                            else
                                currentActiveTag = FileBar;
                        }
                        else
                        {
                            if (currentActiveTag->next)
                                currentActiveTag = currentActiveTag->next;
                            else
                                currentActiveTag = FileBar;
                        }
                    }

                    else
                    {
                        if (key == SDLK_TAB)
                        {
                            if (showCompletion)
                            {
                                replaceWord(CompletionBox.active->text);
                            }
                            else
                            {
                                insertString("    ");
                            }

                            currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                            currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                            currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                            showCompletion = 0;
                        }
                    }

                    if ((mod & SDL_KMOD_CTRL) && key == SDLK_S)
                    {
                        char *content = convertToText(currentActiveTag);
                        writeFile(currentActiveTag->path, content);
                    }

                    if ((mod & SDL_KMOD_CTRL) && key == SDLK_V)
                    {
                        if (SDL_HasClipboardText())
                        {
                            char *s = SDL_GetClipboardText();
                            insertString(s);
                            free(s);
                            // SDL_free(s) in SDL3 usually, but free() might work if aligned.
                            // SDL3 recommends SDL_free.

                            currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                            currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                            currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;
                        }
                    }
                }
            }

            if (event.type == SDL_EVENT_TEXT_INPUT)
            {
                if (currentActiveTag)
                {
                    // insertChar(event.text.text[0]);
                    insertString(event.text.text);
                    currentActiveTag->SELECTION_START_LINE = currentActiveTag->currentLine;
                    currentActiveTag->SELECTION_START_WORD = currentActiveTag->currentWord;
                    currentActiveTag->SELECTION_START_INDEX = currentActiveTag->startIndex;

                    getCompletion(currentActiveTag->currentWord->content, currentActiveTag->startIndex);
                }
            }
        }

        if (IS_SELECTING)
        {
            if (MOUSE_Y > WINDOW_H - FOOTER_H && currentActiveTag->currentLine->next)
            {
                currentActiveTag->EDITOR_SCROLL_Y -= EDITOR_FONT_HEIGHT * 2;
            }

            if (MOUSE_Y < 2 * TOPNAV_H && currentActiveTag->currentLine->prev)
            {
                currentActiveTag->EDITOR_SCROLL_Y += EDITOR_FONT_HEIGHT * 2;
            }
        }

        if (currentActiveTag && currentActiveTag->currentLine && currentActiveTag->EDITOR_SCROLL_Y > 0)
        {
            currentActiveTag->EDITOR_SCROLL_Y = 0;
        }

        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
        SDL_RenderClear(renderer);
        // SDL_SetRenderDrawColor(renderer, 17, 17, 17, 170);
        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 200);

        if (!FileBar)
        {
            SDL_RenderTexture(renderer, bgImage, NULL, &bgRect);
            // SDL_RenderFillRect(renderer, &bgRect);
        }

        /* ===== Draw Home Screen ===== */

        renderTextEditor();
        renderSuggestionBox();
        renderFileBar();

        renderMenuBar();
        renderTopNav();

        if (TOPNAV_RIGHT[0].clicked)
        {
            renderRightPanel();
        }

        renderFooter();
        renderTopNavBarMenu();

        SDL_SetRenderDrawColor(renderer, 155, 20, 127, 15);
        SDL_RenderFillRect(renderer, &fullScreenRect);

        // Render Props

        SDL_RenderTextureRotated(
            renderer,
            prop1,
            NULL,
            &propRect1,
            propangle1,
            &propCenter1,
            SDL_FLIP_NONE);

        // Update Props
        propRect1.y += 1;
        propRect1.x = 100 + sin((time + 0.0) / 100) * 100;
        // propCenter1.y += 1.0;
        propangle1 = sin((time + 0.0) / 100) * 360 + 45;

        if (propRect1.y > WINDOW_H)
            propRect1.y = -propRect1.h;

        time++;
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    SDL_DestroySurface(bgImageSurface);
    SDL_DestroyTexture(bgImage);

    SDL_StopTextInput(window);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
