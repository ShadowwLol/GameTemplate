#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/def.h"
#include "../include/logs.h"
#define MISC_RANDOM
#include "../include/misc.h"


#if __WIN32
int main_menu(HANDLE hConsole, WORD saved_attributes, SDL_Renderer * rend, SDL_Window * win);
#else
int main_menu(SDL_Renderer * rend, SDL_Window * win);
#endif

#if __WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
#else
int main(void){
#endif
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_GAMECONTROLLER) != 0)
    {
        log(ERR, "Failed initializing SDL : %s", SDL_GetError());
        SDL_Quit();
    }else{
        log(SUCCESS, "Successfully initialized SDL");
    }

    SDL_Window* win = SDL_CreateWindow(WINDOW_TITLE,
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!win){
        log(ERR, "Failed creating window : %s", SDL_GetError());
        SDL_Quit();
    }else{
        log(SUCCESS, "Successfully created window");
    }

    // create a renderer, which sets up the graphics hardware
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!rend){
        log(ERR, "Failed creating renderer : %s", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
    }else{
        log(SUCCESS, "Successfully created renderer");
    }

    if(SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND) != 0){
        log(ERR, "Failed configuring renderer blend mode : %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }else{
        log(SUCCESS, "Successfully configured renderer blend mode");
    }

    int current_display = SDL_GetWindowDisplayIndex(win);
    SDL_DisplayMode current_dp;
    if (0 != SDL_GetCurrentDisplayMode(current_display, &current_dp)){
        log(ERR, "Failed getting display mode for video display #%d : %s", current_display, SDL_GetError());
    }else{log(INFO, "Display #%d: [%dx%dpx] @ %dhz", current_display, current_dp.w, current_dp.h, current_dp.refresh_rate);}

    SDL_SetWindowMinimumSize(win, (current_dp.w/2), (current_dp.h/2));
    SDL_SetWindowMaximumSize(win, current_dp.w, current_dp.h);
    SDL_RenderSetLogicalSize(rend, WINDOW_WIDTH, WINDOW_HEIGHT);
    Size WindowSize = {WINDOW_WIDTH, WINDOW_HEIGHT};

    log(INFO, "Target frame time : [%.2fms]", 1000.0f/FPS);

    SDL_RaiseWindow(win);

    SDL_GameController * game_controller = NULL;
    int i = 0;
    for (i = 0; i < SDL_NumJoysticks(); i++){
        if (SDL_IsGameController(i)){
            game_controller = SDL_GameControllerOpen(i);
            break;
        }
    }

    // UI
    int close_requested = 0;
    int has_focus = 1;
    int paused = 0;

    int current_button = -1;

    /* Main Menu */
    #if __WIN32
    if (0 != main_menu(hConsole, saved_attributes, rend, win)){
    #else
    if (0 != main_menu(rend, win)){
    #endif
        close_requested = 1;
    }
    /* * * * * * */

    // load the image into memory
    /* SDL_Texture * texture = IMG_LoadTexture(rend, "resources/image.png"); */

    // Fonts
    FC_Font* font = FC_CreateFont();
    FC_LoadFont(font, rend, "resources/bee.ttf", 20, FC_MakeColor(0,255,160,255), TTF_STYLE_NORMAL);

    // Fps timing
    clock_t startm, stopm;

    double beforeTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
    float current_fps = -0.0;
    float time_since_last_fps = 0.0;

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
                    close_requested = 1;
                    break;
                
                case SDL_CONTROLLERBUTTONUP:
                    switch(event.cbutton.button){
                        case SDL_CONTROLLER_BUTTON_GUIDE:
                            if (!paused){paused = 1;}else{paused = 0; current_button = -1;}
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                            if (paused && current_button < 1){current_button++;}
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_UP:
                            if (paused && current_button > 0){current_button--;};
                            break;

                        case SDL_CONTROLLER_BUTTON_A:
                            if (paused){
                                switch(current_button){
                                    case 0:
                                        /* Resume */
                                        paused = 0;
                                        break;
                                    case 1:
                                        /* Quit */
                                        close_requested = 1;
                                        break;
                                    default:
                                        break;
                                }
                            }

                        default:
                            break;
                    }

                case SDL_KEYUP:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            if (!paused){paused = 1;}else{paused = 0; current_button = -1;}
                            break;

                        case SDL_SCANCODE_F11:
                            ToggleFullscreen(win);
                            FC_LoadFont(font, rend, "resources/bee.ttf", 20, FC_MakeColor(0,255,160,255), TTF_STYLE_NORMAL);
                            break;
                        
                        case SDL_SCANCODE_DOWN:
                            if (paused && current_button < 1){
                                current_button++;
                            }
                            break;
                        
                        case SDL_SCANCODE_UP:
                            if (paused && current_button > 0){
                                current_button--;
                            }
                            break;
                        
                        case SDL_SCANCODE_RETURN:
                            if (paused){
                                switch(current_button){
                                    case 0:
                                        /* Resume */
                                        paused = 0;
                                        current_button = -1;
                                        break;
                                    case 1:
                                        /* Quit */
                                        close_requested = 1;
                                        break;
                                    default:
                                        break;
                                }
                            }
                        default:
                            break;
                    }
                    break;

                case SDL_WINDOWEVENT:
                    switch(event.window.event){
                        case SDL_WINDOWEVENT_RESIZED:
                            FC_LoadFont(font, rend, "resources/bee.ttf", 20, FC_MakeColor(0,255,160,255), TTF_STYLE_NORMAL);
                            break;

                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            has_focus = 1;
                            FC_LoadFont(font, rend, "resources/bee.ttf", 20, FC_MakeColor(0,255,160,255), TTF_STYLE_NORMAL);
                            break;
                        
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                            has_focus = 0;
                            paused = 1;
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

        if (has_focus && !paused){
            // clear the window
            SDL_RenderClear(rend);

        }else if (!has_focus || paused){
            /* Game has been paused */
            SDL_RenderClear(rend);
            SDL_SetRenderDrawColor(rend, 20, 20, 20, 255);
            SDL_Rect pause_box = {0, 0, (WindowSize.w/3.4), (WindowSize.h)};
            SDL_Rect pause_env = {pause_box.w, 0, WINDOW_WIDTH-pause_box.w, pause_box.h};
            SDL_RenderFillRect(rend, &pause_env);

            render_button(rend, win, font, 25, (pause_box.w/2), (pause_box.h/3)/2, current_button, 0, "Resume");
            if (ClickedText(rend, win, font, "Resume", ((float)pause_box.w/2), ((float)pause_box.h/3)/2) == 1){paused = 0; current_button = -1;}

            render_button(rend, win, font, 25, (pause_box.w/2), ((pause_box.h/3)/2)+100, current_button, 1, "Quit?");
            if (ClickedText(rend, win, font, "Quit?", ((float)pause_box.w/2), (((float)pause_box.h/3)/2)+100) == 1){close_requested = 1;}
        }

        // wait 1/60th of a second
        if ( (stopm = clock()) == -1){err("Failed ending fps timer.");}

        SDL_Delay((1000.0/FPS)-(((double)stopm-startm)/CLOCKS_PER_SEC));
        FC_Draw(font, rend, 0, 0, "[%.0fFPS]", current_fps);

        SDL_RenderPresent(rend);
    }

    // clean up resources before exiting
    printf("Quitting..\n");
    if (game_controller != NULL){SDL_GameControllerClose(game_controller);}
    FC_FreeFont(font);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_SUCCESS;
}