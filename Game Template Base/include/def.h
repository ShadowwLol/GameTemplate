#ifndef DEF_H
#define DEF_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include "../include/SDL_FontCache.h"


/* Configuration  */
#define DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);

/* Width of the Window */
#define WINDOW_WIDTH (1280)
/* Height of the Window */
#define WINDOW_HEIGHT (720)
/* Title of the Window */
#define WINDOW_TITLE "My Game!"
/* Window aspect ratio */
#define ASPECT_RATIO (16.f/9.f)
/* Frames per second */
#define FPS (60)
/* * * * * */

typedef enum {
    TILE_DIRT = 0, TILE_GRASS = 1, TILE_ROCKS = 2
} tileset;

typedef struct Vector2{
    int x;
    int y;
} Vector2;

typedef struct Vector2f{
    float x;
    float y;
} Vector2f;

typedef struct Size{
    int w;
    int h;
} Size;

typedef struct Image{
    char * path;
    SDL_Texture * texture;
    SDL_Rect src;
    SDL_Rect dest;
} Image;

typedef struct Spritesheet{
    char * path;
    SDL_Texture * texture;
    unsigned int hframes;
    unsigned int vframes;
    SDL_Rect src;
    SDL_Rect dest;
} Spritesheet;

typedef struct Color{
    double r;
    double g;
    double b;
    double a;
} Color;

#if __WIN32
#include <windows.h>
void logs(HANDLE hConsole, WORD saved_attributes, int l, const char * format, ...);
#else
void logs(int l, const char * format, ...);
#endif

void ToggleFullscreen(SDL_Window* Window);
void font_create(SDL_Renderer *rend, char * path, SDL_Texture * texture, SDL_Rect rect, unsigned int size, unsigned int x, unsigned int y, char * text, SDL_Color color);
int Ishovering( SDL_Renderer * rend, SDL_Window * win, int x, int y, unsigned int width, unsigned int height );
int Clicked(SDL_Renderer * rend, SDL_Window * win, int x, int y, unsigned int width, unsigned int height);
int ClickedText(SDL_Renderer * rend, SDL_Window * win, FC_Font * font, const char * text, float x, float y);
SDL_Rect render_button(SDL_Renderer * rend, SDL_Window * win, FC_Font * font, unsigned int ptsize, unsigned int x, unsigned int y, int current_button, int index, const char * text);
Vector2 get_global_mouse_position();
Vector2f get_logical_mouse_position(SDL_Renderer *renderer, SDL_Window *window);

#endif