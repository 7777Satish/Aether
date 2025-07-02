#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct{
    int x;
    int y;
    int w;
    int h;
    int position;
    char type[20];
    char button_text[30];
    SDL_Color button_bg;
    SDL_Color button_fg;
    SDL_Texture* button_bg_texture;
    SDL_Texture* button_fg_texture;
    SDL_Rect button_bg_rect;
    SDL_Rect button_fg_rect;
} Component;

TTF_Font* poppins_regular;
SDL_Renderer* renderer;

int initButton(Component component){
    SDL_Surface* text_surface = TTF_RenderText_Blended(poppins_regular, component.button_text, component.button_fg);
    component.button_fg_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect fg_r = {
        component.x,
        component.y,
        
        
    };
    component.button_fg_rect = fg_r;
}

int initComponent(Component component){
    if(strcmp(component.type, "button") == 0) initButton(component);
}
