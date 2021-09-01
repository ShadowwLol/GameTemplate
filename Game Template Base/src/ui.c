#include "../include/def.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>

SDL_Rect render_button(SDL_Renderer * rend, SDL_Window * win, FC_Font * font, unsigned int ptsize, unsigned int x, unsigned int y, int current_button, int index, const char * text){
    SDL_Rect button_box;

    SDL_Color normal_color = {200, 200, 200, 255};
    SDL_Color hovered_color = {255, 0, 0, 255};

    FC_Scale sc = {1, 1};
    button_box = FC_GetBounds(font, x, y, FC_ALIGN_LEFT, sc, text);
    button_box.x = x -= (button_box.w/2);
    button_box.y = y -= (button_box.h/2);

    if (!Ishovering(rend, win, button_box.x, button_box.y, button_box.w, button_box.h) && current_button != index){
        FC_DrawColor(font, rend, x, y, normal_color, text);
    }else{
        FC_DrawColor(font, rend, x, y, hovered_color, text);
    }

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

    return button_box; // Success!
}
