#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/renderer.h"
#include "files.h"
#include "events.h"
#include "parser.h"

void renderFile();
char *open_folder_dialog();
void *open_folder_thread();

// ======== LEFT MENU EXPLORER ========

int editorState = 0;

int main()
{
    init();

    int i = 0;

    // Loading Screen

    SDL_Rect loadingBgRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
    SDL_RenderFillRect(renderer, &loadingBgRect);

    // SDL_Rect loadingMenuRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderFillRect(renderer, &MENU_bg_rect);

    // SDL_Rect loadingBgRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 27, 27, 27, 255);
    SDL_RenderFillRect(renderer, &TOPNAV_bg_rect);

    SDL_Rect loadingFooterRect = {0, WINDOW_H - FOOTER_H, WINDOW_W, FOOTER_H};
    SDL_SetRenderDrawColor(renderer, 57, 57, 57, 255);
    SDL_RenderFillRect(renderer, &loadingFooterRect);

    // SDL_Surface* bgImageSurface = IMG_Load("assets/bgImage.png");
    SDL_Surface *bgImageSurface = IMG_Load("assets/girl.png");
    if (!bgImageSurface)
    {
        fprintf(stderr, "Error loading background image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Texture *bgImage = SDL_CreateTextureFromSurface(renderer, bgImageSurface);
    if (!bgImage)
    {
        fprintf(stderr, "Error creating background texture: %s\n", SDL_GetError());
        SDL_FreeSurface(bgImageSurface);
        return 1;
    }

    SDL_Rect bgRect = {
        MENU_W,
        0,
        (bgImageSurface->w + 0.0) / bgImageSurface->h * WINDOW_H,
        WINDOW_H};

    SDL_Rect fullScreenRect = {
        0,
        0,
        WINDOW_W,
        WINDOW_H};

    // Load the I-beam (text) cursor
    SDL_Cursor *ibeam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    // SDL_SetCursor(ibeam);

    // Load the Pointer cursor
    SDL_Cursor *arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    // SDL_SetCursor(ibeam);

    int running = 1;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.motion.x;
                int y = event.motion.y;

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
                    SDL_Rect r;
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

                        SDL_Rect r;
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
                        currentActiveTag->startIndex = (x - (sum + MENU_W + 70)) / cursor->w;

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
                        printf("Node Does not Exist\n");
                    }
                }
            }

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    WINDOW_W = event.window.data1;
                    WINDOW_H = event.window.data2;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP)
            {
                IS_MOUSE_DOWN = 0;
                IS_SELECTING = 0;
            }

            if (event.type == SDL_MOUSEMOTION)
            {

                int x = event.motion.x;
                int y = event.motion.y;

                MOUSE_X = x;
                MOUSE_Y = y;

                // Top Nav Left Buttons
                if (y < TOPNAV_H + 50)
                {
                    for (i = 0; i < 5; i++)
                    {
                        TOPNAV_MENU_NODE *node = &TOPNAV_MENU[i];

                        SDL_Rect r;
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

                // Menu Bar Buttons
                if (x < MENU_W)
                {
                    for (i = 0; i < (int)(sizeof(LEFT_MENU) / sizeof(LEFT_MENU[0])); i++)
                    {
                        MENU_BAR_NODE *node = &LEFT_MENU[i];

                        SDL_Rect r;
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
                if (x > 0 && x < MENU_W)
                {
                    if (Folder)
                    {

                        handleExplorerItemsHover(&Folder, x, y);
                    }
                }

                // Text Editor
                if (currentActiveTag && x > MENU_W && x < WINDOW_W - MINIMAP_W && y > 2 * TOPNAV_H && y < WINDOW_H - FOOTER_H)
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
                        if(currentActiveTag->startIndex<0) currentActiveTag->startIndex = 0;
                        // printf("%s %s %d\n", currentActiveTag->currentLine->word->content, currentActiveTag->currentWord->content, currentActiveTag->startIndex);
                    }
                    else
                    {
                        printf("Node Does not Exist\n");
                    }
                }
            
            }

            if (event.type == SDL_MOUSEWHEEL)
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

            if (event.type == SDL_KEYDOWN)
            {
                int key = event.key.keysym.sym;

                if (key == SDLK_BACKSPACE)
                    leftDeleteChar();
                if (key == SDLK_UP)
                    moveCursorUp();
                if (key == SDLK_DOWN)
                    moveCursorDown();
                if (key == SDLK_LEFT)
                    moveCursorLeft();
                if (key == SDLK_RIGHT)
                    moveCursorRight();
                if (key == SDLK_RETURN)
                    createNewline();
            }

            if (event.type == SDL_TEXTINPUT)
            {
                if (currentActiveTag)
                    insertChar(event.text.text[0]);
            }
        }

        if(IS_SELECTING){
            if(MOUSE_Y > WINDOW_H - FOOTER_H && currentActiveTag->currentLine->next){
                currentActiveTag->EDITOR_SCROLL_Y -= EDITOR_FONT_HEIGHT*2;
            }

            if(MOUSE_Y < 2*TOPNAV_H && currentActiveTag->currentLine->prev){
                currentActiveTag->EDITOR_SCROLL_Y += EDITOR_FONT_HEIGHT*2;
            }
        }

        if(currentActiveTag && currentActiveTag->currentLine && currentActiveTag->EDITOR_SCROLL_Y > 0){
            currentActiveTag->EDITOR_SCROLL_Y = 0;
        }

        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
        SDL_RenderClear(renderer);
        // SDL_SetRenderDrawColor(renderer, 17, 17, 17, 170);
        SDL_SetRenderDrawColor(renderer, 17, 17, 17, 200);

        if (!FileBar)
        {
            SDL_RenderCopy(renderer, bgImage, NULL, &bgRect);
            // SDL_RenderFillRect(renderer, &bgRect);
        }

        /* ===== Draw Home Screen ===== */

        renderFileBar();

        renderMenuBar();
        renderTopNav();
        renderTextEditor();
        renderFooter();
        renderTopNavBarMenu();

        SDL_SetRenderDrawColor(renderer, 155, 20, 127, 15);
        SDL_RenderFillRect(renderer, &fullScreenRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    SDL_FreeSurface(bgImageSurface);
    SDL_DestroyTexture(bgImage);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
