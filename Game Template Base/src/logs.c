#include <stdio.h>
#include <stdarg.h>
#include "../include/logs.h"

#if __WIN32
#include <windows.h>

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