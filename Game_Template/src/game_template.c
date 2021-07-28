#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/useful.h"
#include "../include/SDL_FontCache.h"

#define DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);

#if _WIN32
#include <windows.h>
#endif

#define WINDOW_WIDTH (1280)
#define WINDOW_HEIGHT (720)
#define WINDOW_TITLE "Game Template!"
#define FPS (60)

#if __WIN32
void logs(HANDLE hConsole, WORD saved_attributes, int l, const char * format, ...);
#else
void logs(int l, const char * format, ...);
#endif

#if __WIN32
void load_spritesheet(HANDLE hConsole, WORD saved_attributes, SDL_Window * win, SDL_Renderer * rend, Spritesheet * sheet);
#else
void load_spritesheet(SDL_Window * win, SDL_Renderer * rend, Spritesheet * sheet);
#endif

#if __WIN32
void map(HANDLE hConsole, WORD saved_attributes, SDL_Renderer * rend, SDL_Window * win);
#else
void map(SDL_Renderer * rend, SDL_Window * win);
#endif


#if __WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

	logs(hConsole, saved_attributes, INFO, "Target frame time : [%.2fms]", 1000.0f/FPS);
#else
int main(void){
    logs(INFO, "Target frame time : [%.2fms]", 1000.0f/FPS);
#endif

    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }else{
        #if __WIN32
        logs(hConsole, saved_attributes, SUCCESS, "Successfully initialized SDL");
        #else
        logs(SUCCESS, "Successfully initialized SDL");
        #endif
    }

    SDL_Window* win = SDL_CreateWindow(WINDOW_TITLE,
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    check(win, "Failed creating window", "Successfully created window");

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    check(rend, "Failed creating renderer", "Successfully created renderer");
    check(rend, "Failed creating renderer", "Successfully created renderer");
    if(SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND) != 0){
        err("Failed setting render blend mode");
    }

    // load the image into memory
    SDL_Texture * texture;
    image_load(texture, "resources/image.png", rend, win);

    // Spritesheets
    Spritesheet sheet;
    sheet.path = "resources/myspritesheet.png";
    sheet.hframes = 4;
    sheet.vframes = 4;
    #if __WIN32
    load_spritesheet(hConsole, saved_attributes, win, rend, &sheet);
    #else
    load_spritesheet(win, rend, &sheet);
    #endif
    sheet.dest.x = 100;
    sheet.dest.y = (WINDOW_HEIGHT/2)-(sheet.dest.h/2);
    SDL_RendererFlip flipType = SDL_FLIP_NONE;
    const int velocity = 15;

    // Camera
    SDL_Rect camera;

    // Fonts
    FC_Font* font = FC_CreateFont();
    FC_LoadFont(font, rend, "resources/bee.ttf", 20, FC_MakeColor(0,255,160,255), TTF_STYLE_NORMAL);

    // set to 1 when window close button is pressed
    int close_requested = 0;
    int has_focus = 0;
    int paused = 0;

    // Fps timing
    clock_t startm, stopm;
    unsigned int newtime = 0;
    unsigned int oldtime;

    // animation loop
    while (!close_requested)
    {
        //Start fps timer
        if ( (startm = clock()) == -1){err("Failed starting fps timer.");}

        // process events
        SDL_Event event;

        oldtime = newtime;
        newtime = SDL_GetTicks();

        if(oldtime == newtime && !paused) {
            err("Failed counting fps.\n");
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    close_requested = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_W:
                            sheet.src.y = sheet.dest.h;
                            //SDL_Delay(200);
                            if (sheet.src.x != (sheet.dest.w*(sheet.hframes-1))){
                                sheet.src.x += sheet.dest.w;
                            }else{sheet.src.x = 0;}
                            //update_spritesheet(sheet, newtime);
                            sheet.dest.y -= velocity;
                            camera.y += velocity;
                            break;
                        
                        case SDL_SCANCODE_S:
                            sheet.src.y = 0;
                            if (sheet.src.x != (sheet.dest.w*(sheet.hframes-1))){
                                sheet.src.x += sheet.dest.w;
                            }else{sheet.src.x = 0;}
                            //update_spritesheet(sheet, newtime);
                            sheet.dest.y += velocity;
                            camera.y -= velocity;
                            break;

                        case SDL_SCANCODE_D:
                            //Updating spritesheet
                            //flipType = SDL_FLIP_NONE;
                            sheet.src.y = (sheet.dest.h*(sheet.vframes-1));
                            if (sheet.src.x != (sheet.dest.w*(sheet.hframes-1))){
                                sheet.src.x += sheet.dest.w;
                            }else{sheet.src.x = 0;}
                            //update_spritesheet(sheet, newtime);
                            sheet.dest.x += velocity;
                            camera.x -= velocity;
                            break;
                        
                        case SDL_SCANCODE_A:
                            //flipType = SDL_FLIP_HORIZONTAL;
                            //update_spritesheet(sheet, newtime);
                            sheet.src.y = (sheet.dest.h*(sheet.vframes-2));
                            if (sheet.src.x != (sheet.dest.w*(sheet.hframes-1))){
                                sheet.src.x += sheet.dest.w;
                            }else{sheet.src.x = 0;}
                            sheet.dest.x -= velocity;
                            camera.x += velocity;
                            break;

                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            if (!paused){paused = 1;}else{paused = 0;}
                            break;

                        case SDL_SCANCODE_F11:
                            ToggleFullscreen(win);
                            break;

                        default:
                            break;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    switch(event.window.event){
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            has_focus = 1;
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

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

        if (has_focus && !paused){
            // clear the window
            SDL_RenderClear(rend);

            // draw the image to the window
            //SDL_Rect r = {camera.x + 0, camera.y + 0, 1280, 720};
            //SDL_RenderCopy(rend, texture, NULL, &r);
            #if __WIN32
            map(hConsole, saved_attributes, rend, win);
            #else
            map(rend, win);
            #endif
            SDL_RenderCopyEx(rend, sheet.texture, &sheet.src, &sheet.dest, 0, NULL, flipType);
            //printf("Camera : [%d, %d]\n", camera.x, camera.y);
            //printf("Player : [%d, %d]\n", sheet.dest.x, sheet.dest.y);

        }else if (!has_focus || paused){
            /* Game has been paused */
            SDL_Rect pause_box;
            pause_box.h = WINDOW_HEIGHT;
            pause_box.w = (WINDOW_WIDTH/3.4);
            pause_box.x = 0;
            pause_box.y = 0;
            SDL_Rect pause_bg;
            pause_bg.h = WINDOW_HEIGHT;
            pause_bg.w = WINDOW_WIDTH-(WINDOW_WIDTH/3.4);
            pause_bg.x = (WINDOW_WIDTH/3.4);
            pause_bg.y = 0;
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderClear(rend);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderFillRect(rend, &pause_box);
            SDL_SetRenderDrawColor(rend, 20, 20, 20, 255);
            SDL_RenderFillRect(rend, &pause_bg);

            render_button(rend, win, 25, 100, 50, "Resume");
            if (Clicked(win, 100, 50, 25 * strlen("Resume"), 25 * 2) == 1){paused = 0;}

            render_button(rend, win, 25, 100, 120, "Quit?");
            if (Clicked(win, 100, 100, 25 * strlen("Quit?"), 25 * 2) == 1){break;}
        }

        // wait 1/60th of a second
        if ( (stopm = clock()) == -1){err("Failed ending fps timer.");}

        SDL_Delay((1000/FPS)-(((double)stopm-startm)/CLOCKS_PER_SEC));
        FC_Draw(font, rend, 0, 0, "[%.0fFPS]", 1000.0f/(newtime-oldtime));

        SDL_RenderPresent(rend);
    }

    // clean up resources before exiting
    printf("Quitting..\n");
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    FC_FreeFont(font);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void ToggleFullscreen(SDL_Window* Window) {
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    int IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
    SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
    switch (IsFullscreen){ case 1: SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);}
    SDL_ShowCursor(1);
}

#if __WIN32
void logs(HANDLE hConsole, WORD saved_attributes, int l, const char  * format, ...){
    switch(l){
        case WARN:
            set_term_color(hConsole, TERM_YELLOW);
            fprintf(stdout, "[!] ");
            break;
        case SUCCESS:
            set_term_color(hConsole, TERM_GREEN);
            fprintf(stdout, "[+] ");
            break;
        case ERR:
            set_term_color(hConsole, TERM_RED);
            fprintf(stdout, "[-] ");
            break;
        case INFO:
            set_term_color(hConsole, TERM_BLUE);
            fprintf(stdout, "[#] ");
            break;
        default:
            break;
    }
    clear_term_color(hConsole, saved_attributes);
#else
void logs(int l, const char * format, ...){
    switch(l){
        case WARN:
            set_term_color(TERM_YELLOW);
            fprintf(stdout, "[!] ");
            break;
        case SUCCESS:
            set_term_color(TERM_GREEN);
            fprintf(stdout, "[+] ");
            break;
        case ERR:
            set_term_color(TERM_RED);
            fprintf(stdout, "[-] ");
            break;
        case INFO:
            set_term_color(TERM_BLUE);
            fprintf(stdout, "[#] ");
            break;
        default:
            break;
    }
    clear_term_color();
#endif
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    putchar('\n');
    va_end(args);
}

int render_button(SDL_Renderer * rend, SDL_Window * win, unsigned int ptsize, unsigned int x, unsigned int y, const char * text){
    SDL_Rect button_box;
    button_box.h = ptsize * 2;
    button_box.w = ptsize * strlen(text);
    button_box.x = x-(button_box.w/3);
    button_box.y = y-(button_box.h/3);

    if (text){
        FC_Font * font = FC_CreateFont();
        if (!Ishovering(win, button_box.x, button_box.y, button_box.w, button_box.h)){
            FC_LoadFont(font, rend, "resources/bee.ttf", ptsize, FC_MakeColor(200,200,200,255), TTF_STYLE_NORMAL);
        }else{
            if (Clicked(win, button_box.x, button_box.y, button_box.w, button_box.h) == 1){
                /* Left mouse button clicked */
                FC_LoadFont(font, rend, "resources/bee.ttf", ptsize, FC_MakeColor(230,0,0,255), TTF_STYLE_NORMAL);
            }else{
                FC_LoadFont(font, rend, "resources/bee.ttf", ptsize, FC_MakeColor(255,0,0,255), TTF_STYLE_NORMAL);
            }
        }
        FC_Draw(font, rend, x, y, text);
    }

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
    SDL_RenderFillRect(rend, &button_box);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

    return 1; // Success!
}

#if __WIN32
void load_spritesheet(HANDLE hConsole, WORD saved_attributes, SDL_Window * win, SDL_Renderer * rend, Spritesheet * sheet){
#else
void load_spritesheet(SDL_Window * win, SDL_Renderer * rend, Spritesheet * sheet){
#endif
    image_load(sheet->texture, sheet->path, rend, win);
    
    SDL_QueryTexture(sheet->texture, NULL, NULL, &sheet->dest.w, &sheet->dest.h);
    sheet->dest.w /= sheet->hframes;
    sheet->dest.h /= sheet->vframes;
    //sheet.dest.x = 100;
    //sheet.dest.y = 100;
    
    sheet->src.x = 0;
    sheet->src.y = 0;
    sheet->src.h = sheet->dest.h;
    sheet->src.w = sheet->dest.w;
}

#if __WIN32
void map(HANDLE hConsole, WORD saved_attributes, SDL_Renderer * rend, SDL_Window * win){
#else
void map(SDL_Renderer * rend, SDL_Window * win){
#endif
    /* An integer 2D array to hold our map each integer representing a 32x32 texture */
    /* 1280 / 32 = 22.5 = 23 ; 720 / 32 = 40 */
    int world[23][40] = {
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        };
    SDL_Texture * dirt;
    image_load(dirt, "resources/dirt.png", rend, win);
    SDL_Texture * grass;
    image_load(grass, "resources/grass.png", rend, win);
    SDL_Texture * rocks;
    image_load(rocks, "resources/rocks.png", rend, win);

    SDL_Rect dest;
    dest.w = 32;
    dest.h = 32;
    unsigned int row = 0;
    unsigned int column = 0;
    for (row = 0; row < 23; row++){
        for (column = 0; column < 40; column++){
            dest.x = column * 32;
            dest.y = row * 32;
            switch(world[row][column]){
                case TILE_DIRT:
                    SDL_RenderCopy(rend, dirt, NULL, &dest);
                    break;
                case TILE_GRASS:
                    SDL_RenderCopy(rend, grass, NULL, &dest);
                    break;
                case TILE_ROCKS:
                    SDL_RenderCopy(rend, rocks, NULL, &dest);
                    break;
                default:
                    break; // log "Undefined map value at world[row][column]"
            }
        }
    }
}