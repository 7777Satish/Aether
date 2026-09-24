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
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
    SDL_RenderFillRect(renderer, &loadingBgRect);

    // SDL_FRect loadingMenuRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderFillRect(renderer, &MENU_bg_rect);

    // SDL_FRect loadingBgRect = {0, 0, WINDOW_W, WINDOW_H};
    SDL_SetRenderDrawColor(renderer, 27, 27, 27, 255);
    SDL_RenderFillRect(renderer, &TOPNAV_bg_rect);

    SDL_SetRenderDrawColor(renderer, 57, 57, 57, 255);
    SDL_RenderFillRect(renderer, &loadingFooterRect);


    int running = 1;
    int time = 0;
    SDL_Event event;

    int dirty = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            handleEvents(event, &dirty, &running);
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
        // SDL_Delay(1000 / 60);
    }

    SDL_DestroySurface(bgImageSurface);
    SDL_DestroyTexture(bgImage);

    SDL_StopTextInput(window);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
