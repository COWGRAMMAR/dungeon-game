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
    game();

    wrefresh(arena);
    getch();
    endwin();

    return 0;
}