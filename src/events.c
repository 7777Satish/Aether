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
        targetY = &EXPLORER_SCROLL_Y;
    }
    else if (FileBar != NULL && MOUSE_X > MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H * 2)
    {
        targetX = &EDITOR_SCROLL_X;
        targetY = &EDITOR_SCROLL_Y;
        horizontal = 1;
    }
    else if (currentActiveTag != NULL && MOUSE_X > MENU_BAR_W + MENU_W && MOUSE_Y > TOPNAV_H && MOUSE_Y < TOPNAV_H * 2)
    {
        targetX = &FILEMENU_SCROLL_X;
        horizontal = 2;
    }

    if (horizontal != 2 && targetY != NULL)
    {
        if (y > 0)
        {
            *targetY += dy;
            if (*targetY > 0)
                *targetY = 0;
        }
        else if (y < 0)
        {
            *targetY -= dy;
        }
    }

    if (horizontal != 0 && targetX != NULL)
    {
        if (x > 0)
        {
            *targetX -= dx;
        }
        else if (x < 0)
        {
            *targetX += dx;
            if (*targetX > 0)
                *targetX = 0;
        }
    }
}