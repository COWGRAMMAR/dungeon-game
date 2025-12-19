#include <curses.h>
#include <windows.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "dungeon.h"

int screenwidth = ((200 * 2) + 1);
int screenheight = 51;

WINDOW *win;

void set_console_char_size(short cols, short rows)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    // step 1: Increase the buffer first (safe)
    COORD bufferSize = {cols, rows};
    SetConsoleScreenBufferSize(hOut, bufferSize);

    // step 2: Set the window size according to the buffer
    SMALL_RECT windowSize = {0, 0, cols - 1, rows - 1};
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);

    // step 3: lock resize & maximize
    HWND hwnd = GetConsoleWindow();
    if (!hwnd)
        return;

    // setup feature for windows terminal
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX; // remove maximize button
    style &= ~WS_SIZEBOX;     // disabled resize via drag
    SetWindowLong(hwnd, GWL_STYLE, style);

    // idk man what is this i just ai it for this whole function🥀🥀🥀
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE |
                     SWP_NOZORDER | SWP_FRAMECHANGED);
}

void init()
{
    // initialize extended ascii character
    setlocale(LC_ALL, "");

    // initialize random generator
    srand(time(NULL));

    // initialize window
    win = initscr();

    // player input
    keypad(win, TRUE);
    noecho();
    curs_set(0);

    // initializze colors
    if (has_colors() == false)
    {
        endwin();
        fprintf(stderr, "Your terminal does not support color\n");
        exit(1);
    }
    start_color();
}