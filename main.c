#include <curses.h>
#include <windows.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "dungeon.h"

int main(int argc, char const *argv[])
{

    // process user args
    if (argc == 1)
    {
    }
    else if (argc == 3)
    {
        if (!strcmp(argv[1], "-d"))
        {
            if (sscanf(argv[2], "%dx%d", &screenwidth, &screenheight) != 2)
            {
                printf("Usage: snake [options]\nOptions:\n -d [width]x[height]"
                       "\tdefine dimensions of the screen\n\nDefault dimensions are 59x25\n");
                exit(1);
            }
        }
    }
    else
    {
        printf("Usage: snake [options]\nOptions:\n -d [width]x[height]"
               "\tdefine dimensions of the screen\n\nDefault dimensions are 59x25\n");
        exit(1);
    }

    set_console_char_size(screenwidth, screenheight);
    init();

    // set up windows terimnal and initialize game
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    mvprintw(0, 0, "Ukuran yang diminta: %dx%d", screenwidth, screenheight);
    mvprintw(0, 0, "Ukuran aktual: %dx%d", max_x, max_y);
    mvprintw(0, 0, "halo semua - Tekan sembarang tombol untuk keluar");
    
    refresh();
    getch();
    
    endwin();
    return 0;
}