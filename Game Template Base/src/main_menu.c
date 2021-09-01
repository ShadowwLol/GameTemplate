#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "../include/def.h"
#include "../include/logs.h"

#if __WIN32
int main_menu(HANDLE hConsole, WORD saved_attributes, SDL_Renderer * rend, SDL_Window * win){
#else
int main_menu(SDL_Renderer * rend, SDL_Window * win){
#endif

    // Fonts
    FC_Font* font = FC_CreateFont();
    FC_Font* bold_font = FC_CreateFont();
    FC_LoadFont(font, rend, "resources/bee.ttf", 20, FC_MakeColor(0,255,160,255), TTF_STYLE_NORMAL);
    FC_LoadFont(bold_font, rend, "resources/bee.ttf", 50, FC_MakeColor(0,255,160,255), TTF_STYLE_BOLD);

    /* Animation */
    SDL_Color menu_color = {20, 20, 20, 0};
    Vector2f title_pos = {((float)WINDOW_WIDTH/2), (-200)};
    SDL_bool title_has_fallen = SDL_FALSE;

    // set to 1 when window close button is pressed
    int close_requested = 0;

    // Fps timing
    clock_t startm, stopm;

    double beforeTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
    float current_fps = -0.0;
    float time_since_last_fps = 0.0;

    int return_value = 0;

    // animation loop
    while (!close_requested)
    {
        //Start fps timer
        if ( (startm = clock()) == -1){err("Failed starting fps timer.");}

        // process events
        SDL_Event event;
        
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    return_value = 1;
                    close_requested = 1;
                    break;
                
                case SDL_CONTROLLERDEVICEADDED:
                    printf("Controller plugged in : %d\n", event.cdevice.which);
                    break;

                case SDL_CONTROLLERBUTTONUP:
                    switch(event.cbutton.button){
                        case SDL_CONTROLLER_BUTTON_START:
                            return_value = 0;
                            close_requested = 1;
                            break;
                        default:
                            break;
                    }

                case SDL_KEYUP:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_RETURN:
                            return_value = 0;
                            close_requested = 1;
                            break;

                        case SDL_SCANCODE_F11:
                            ToggleFullscreen(win);
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        
        /* Fps and delta */
        double currentTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
        float deltaTime = (float)(currentTime - beforeTime);
        beforeTime = currentTime; // Prime beforeTime for the next frame

        time_since_last_fps += deltaTime;
        if (time_since_last_fps >= 0.25F || current_fps <= 0.0F) {
            time_since_last_fps = 0.0F;
            current_fps = 1.0 / deltaTime;
        }
        /* * * * * * * * */

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

        // clear the window
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, menu_color.r, menu_color.g, menu_color.b, menu_color.a);
        //menu_color.a = lerp(menu_color.a, 255, 100, deltaTime);

        if (menu_color.a < 255){
            menu_color.a += 100 * deltaTime;
        }else{menu_color.a = 255;}

        if (title_pos.y < ((float)WINDOW_HEIGHT/2) && !title_has_fallen){
            title_pos.y += 100 * deltaTime;
        }else{title_has_fallen = SDL_TRUE;}

        if (title_pos.y >= (((float)WINDOW_HEIGHT/2)-200) && title_has_fallen){
            title_pos.y -= 100 * deltaTime;
        }else {title_has_fallen = SDL_FALSE;}
/*
        if (title_has_fallen == SDL_FALSE){
            title_pos.y = lerp(title_pos.y, (WINDOW_HEIGHT/2), 200, deltaTime);
        }else{
            title_pos.y = lerp(title_pos.y, -200, 200, deltaTime);
        }
  */      
        SDL_Rect menu = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(rend, &menu);
        FC_DrawAlign(bold_font, rend, (float)title_pos.x, (float)title_pos.y, FC_ALIGN_CENTER, "%s", WINDOW_TITLE);
        FC_Draw(font, rend, 0, 0, "[%.0fFPS]", current_fps);



        // wait 1/60th of a second
        if ( (stopm = clock()) == -1){err("Failed ending fps timer.");}

        SDL_Delay((1000.0/FPS)-(((double)stopm-startm)/CLOCKS_PER_SEC));

        SDL_RenderPresent(rend);
    }

    // clean up resources before exiting
    printf("Quitting Main Menu..\n");
    FC_FreeFont(font);
    return return_value;
}