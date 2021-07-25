#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/useful.h"
#include "../include/SDL_FontCache.h"
#if _WIN32
#include <windows.h>
#endif

Vector2 get_global_mouse_position(SDL_Window * win){
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    int IsFullscreen = SDL_GetWindowFlags(win) & FullscreenFlag;
    if (IsFullscreen == 1){ // Not fullscreen
        Vector2 mouse_pos;
        SDL_PumpEvents();
        SDL_GetGlobalMouseState(&mouse_pos.x, &mouse_pos.y);
        return mouse_pos;
    }else{
        Vector2 mouse_pos;
        Vector2 win_pos;
        SDL_GetWindowPosition(win, &win_pos.x, &win_pos.y);
        SDL_PumpEvents();
        SDL_GetGlobalMouseState(&mouse_pos.x, &mouse_pos.y);
        mouse_pos.x -= win_pos.x;
        mouse_pos.y -= win_pos.y;
        return mouse_pos;
    }
}

int Ishovering( SDL_Window * win, int x, int y, unsigned int width, unsigned int height ){
    Vector2 mouse_pos = get_global_mouse_position(win);
    if (mouse_pos.x >= x && mouse_pos.x <= (x+width) && mouse_pos.y >= y && mouse_pos.y <= (y+height)){
        return 1;
    }else{return 0;}
}

int Clicked(SDL_Window * win, int x, int y, unsigned int width, unsigned int height){
    if (Ishovering(win, x, y, width, height)){
        Vector2 mouse_pos;
        Uint32 buttons;
        SDL_PumpEvents();
        buttons = SDL_GetGlobalMouseState(&mouse_pos.x, &mouse_pos.y);
        return buttons;
        /*
        1 : LEFT_MOUSE_BUTTON;
        2 : MOUSE_WHEEL
        3 : ?
        4 : RIGHT_MOUSE_BUTTON
        */
    }
    return 0;
}