#include "../include/def.h"
#include <SDL2/SDL_mouse.h>

Vector2 get_global_mouse_position(){
    Vector2 mouse_pos;
    SDL_PumpEvents();
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    return mouse_pos;
}

Vector2f get_logical_mouse_position(SDL_Renderer *renderer, SDL_Window *window){
    Size win_size;
    Size rReal;
    Vector2f rScale;
    Vector2f logicalMouse;

    SDL_GetWindowSize(window, &win_size.w, &win_size.h);

    SDL_RenderGetScale(renderer, &rScale.x, &rScale.y);
    rReal.w = (float)WINDOW_WIDTH*(float)rScale.x;
    rReal.h = (float)WINDOW_HEIGHT*(float)rScale.y;
    
    Vector2 r = {((win_size.w/2) - (rReal.w/2)), ((win_size.h/2) - (rReal.h/2))};

    Vector2 realMouse = get_global_mouse_position();
    Vector2f adjustedMouse = {realMouse.x - r.x, realMouse.y - r.y}; // takes into account any border when keeping aspect ratio
    logicalMouse.x = (float)adjustedMouse.x / (float)rReal.w * (float)WINDOW_WIDTH;
    logicalMouse.y = (float) adjustedMouse.y / (float)rReal.h * (float)WINDOW_HEIGHT;

    return logicalMouse;
}

int Ishovering( SDL_Renderer * rend, SDL_Window * win, int x, int y, unsigned int width, unsigned int height ){
    Vector2f mouse_pos = get_logical_mouse_position(rend, win);
    if (mouse_pos.x >= x && mouse_pos.x <= (x+width) && mouse_pos.y >= y && mouse_pos.y <= (y+height)){
        return 1;
    }else{return 0;}
}

int ClickedText(SDL_Renderer * rend, SDL_Window * win, FC_Font * font, const char * text, float x, float y){
    SDL_Rect button_box;
    FC_Scale sc = {1, 1};
    button_box = FC_GetBounds(font, x, y, FC_ALIGN_LEFT, sc, text);
    if (Ishovering(rend, win, button_box.x, button_box.y, button_box.w, button_box.h)){
        Vector2 mouse_pos;
        Uint32 buttons;
        SDL_PumpEvents();
        buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
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


int Clicked(SDL_Renderer * rend, SDL_Window * win, int x, int y, unsigned int width, unsigned int height){
    if (Ishovering(rend, win, x, y, width, height)){
        Vector2 mouse_pos;
        Uint32 buttons;
        SDL_PumpEvents();
        buttons = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
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