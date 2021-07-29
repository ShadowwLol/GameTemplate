#include <stdio.h>
#include <stdlib.h>

void ToggleFullscreen(SDL_Window* Window);
void font_create(SDL_Renderer *rend, char * path, SDL_Texture * texture, SDL_Rect rect, unsigned int size, unsigned int x, unsigned int y, char * text, SDL_Color color);
int render_button(SDL_Renderer * rend, SDL_Window * win, unsigned int ptsize, unsigned int x, unsigned int y, const char * text);

typedef enum {
	WARN, SUCCESS, ERR, INFO
} logging;

typedef enum {
    TILE_DIRT = 0, TILE_GRASS = 1, TILE_ROCKS = 2
} tileset;

typedef struct Vector2{
    int x;
    int y;
} Vector2;

typedef struct Spritesheet{
    char * path;
    SDL_Texture * texture;
    unsigned int hframes;
    unsigned int vframes;
    SDL_Rect src;
    SDL_Rect dest;
} Spritesheet;

Vector2 get_global_mouse_position(SDL_Window * win);
int Ishovering( SDL_Window * win, int x, int y, unsigned int width, unsigned int height );
int Clicked(SDL_Window * win, int x, int y, unsigned int width, unsigned int height);

#if __WIN32

#define TERM_GREEN FOREGROUND_GREEN
#define TERM_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define TERM_RED FOREGROUND_RED
#define TERM_BLUE FOREGROUND_BLUE

#define set_term_color(CONSOLE, TERM_COLOR){\
	SetConsoleTextAttribute(CONSOLE, TERM_COLOR);\
}

#define clear_term_color(CONSOLE, ATR){\
    SetConsoleTextAttribute(CONSOLE, ATR);\
}

#else

#define TERM_GREEN "\033[32m"
#define TERM_YELLOW "\033[33m"
#define TERM_RED "\033[31m"
#define TERM_BLUE "\033[34m"

#define set_term_color(TERM_COLOR){\
	printf(TERM_COLOR);\
}

#define clear_term_color(){\
    printf("\033[0m");\
}

#endif

#if __WIN32

#define log(...){logs(hConsole, saved_attributes, ##__VA_ARGS__);}

#define err(msg, ...){\
    logs(hConsole, saved_attributes, ERR, msg, ##__VA_ARGS__);\
    logs(hConsole, saved_attributes, ERR, SDL_GetError());\
    /*fprintf(stderr, "[-] Error: [%d, %s()] %s : {%s}\n", __LINE__, __func__, msg, SDL_GetError()); goto end;}*/\
}

#else

#define log logs

#define err(msg, ...){\
    logs(ERR, msg, ##__VA_ARGS__);\
    logs(ERR, SDL_GetError());\
    /*fprintf(stderr, "[-] Error: [%d, %s()] %s : {%s}\n", __LINE__, __func__, msg, SDL_GetError()); goto end;}*/\
}

#endif

#define clamp(val, min, max){if (val < min){val = min;}else if (val > max){val = max;}}
