#include "events.h"

void handleMouseScroll(int x, int y)
{

    int dy = 15;
    int dx = 15;

    int *targetY = NULL;
    int *targetX = NULL;
    int horizontal = 0;

    if (Folder != NULL && MOUSE_X > MENU_BAR_W && MOUSE_X < MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H)
    {
        if (y > 0)
        {
            EXPLORER_SCROLL_Y += dy;
            if (EXPLORER_SCROLL_Y > 0)
                EXPLORER_SCROLL_Y = 0;
        }
        else if (y < 0)
        {
            EXPLORER_SCROLL_Y -= dy;
        }
    }
    else if (FileBar != NULL && MOUSE_X > MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H * 2)
    {

        if (y > 0)
        {
            EDITOR_SCROLL_Y += dy;
            if (EDITOR_SCROLL_Y > 0)
                EDITOR_SCROLL_Y = 0;
        }
        else if (y < 0)
        {
            EDITOR_SCROLL_Y -= dy;
        }


        if (x > 0)
        {
            EDITOR_SCROLL_X -= dx;
            // if(EDITOR_SCROLL_X<-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W) EDITOR_SCROLL_X=-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W;
        }
        else if (x < 0)
        {
            EDITOR_SCROLL_X += dx;
            if (EDITOR_SCROLL_X > 0)
                EDITOR_SCROLL_X = 0;
        }
    }
    else if (currentActiveTag != NULL && MOUSE_X > MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H && MOUSE_Y < TOPNAV_H * 2)
    {

        if (x > 0)
        {
            FILEMENU_SCROLL_X -= dx;
            if(FILEMENU_SCROLL_X<-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W) FILEMENU_SCROLL_X=-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W;
        }
        else if (x < 0)
        {
            FILEMENU_SCROLL_X += dx;
            if (FILEMENU_SCROLL_X > 0)
                FILEMENU_SCROLL_X = 0;
        }
    }

    
    // if (horizontal != 2 && targetY != NULL)
    // {
    //     if (y > 0)
    //     {
    //         *targetY += dy;
    //         if (*targetY > 0)
    //             *targetY = 0;
    //     }
    //     else if (y < 0)
    //     {
    //         *targetY -= dy;
    //     }
    // }

    // if (horizontal != 0 && targetX != NULL)
    // {
    //     if (x > 0)
    //     {
    //         *targetX -= dx;
    //         if(*targetX<-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W) *targetX=-TotalFileBarLength + WINDOW_W-MENU_BAR_W-MENU_W;
    //     }
    //     else if (x < 0)
    //     {
    //         *targetX += dx;
    //         if (*targetX > 0)
    //             *targetX = 0;
    //     }
    // }
}