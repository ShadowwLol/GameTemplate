#ifndef LOGS_H
#define LOGS_H

typedef enum {
	WARN, SUCCESS, ERR, INFO
} logging;

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

#define log(...){logs(hConsole, saved_attributes, ##__VA_ARGS__);}

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

#define log logs

#endif

#define err(msg, ...){\
    log(ERR, msg, ##__VA_ARGS__);\
    log(ERR, SDL_GetError());\
}

#endif