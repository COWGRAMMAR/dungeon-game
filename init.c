#include <curses.h>
#include <windows.h>
#include <locale.h>
#include <time.h>
#include "dungeon.h"

int screenwidth = 150;
int screenheight = 40;

WINDOW *win;

// initialize windows terminal funcgtion
void set_console_char_size(short cols, short rows)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    // step 1: Increase the buffer first (safe)
    COORD bufferSize = {cols, rows};
    SetConsoleScreenBufferSize(hOut, bufferSize);

    // step 2: Set the window size according to the buffer
    SMALL_RECT windowSize = {0, 0, cols, rows};
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

// initialize terminal function
void init()
{
    // initialize extended ascii character
    setlocale(LC_ALL, "");

    // initialize random generator
    srand(time(NULL));

    // initialize window
    win = initscr();

    resize_term(screenheight, screenwidth);

    // player input
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_RED, COLOR_BLACK);

    clear();
    refresh();
}

// tester function for setup window terminal
void tester()
{
    // set up windows terimnal and initialize game
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Clear screen
    clear();

    // Tampilkan informasi
    attron(A_BOLD);
    mvprintw(0, 0, "=== TEST UKURAN TERMINAL ===");
    attroff(A_BOLD);

    mvprintw(2, 2, "Ukuran yang diminta: %d lebar x %d tinggi", screenwidth, screenheight);
    mvprintw(3, 2, "Ukuran aktual curses: %d lebar x %d tinggi", max_x, max_y);

    mvprintw(5, 2, "Coba tekan beberapa tombol:");
    mvprintw(6, 4, "- Tekan 'q' untuk keluar");
    mvprintw(7, 4, "- Tekan arrow keys untuk test");
    mvprintw(8, 4, "- Tekan space bar");

    // Gambar border di sekitar window
    box(stdscr, 0, 0);

    // Tampilkan petunjuk di bagian bawah
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(max_y - 2, 2, "Tekan 'q' untuk keluar dari program");
    attroff(COLOR_PAIR(3) | A_BOLD);
    // Draw a box to visualize boundaries
    for (int x = 0; x < max_x; x++)
    {
        mvaddch(5, x, '-');
        mvaddch(max_y - 5, x, '-');
    }
    for (int y = 5; y < max_y - 4; y++)
    {
        mvaddch(y, 0, '|');
        mvaddch(y, max_x - 1, '|');
    }

    // Instructions
    attron(COLOR_PAIR(3));
    mvprintw(max_y - 3, 2, "Press ANY key to exit");
    attroff(COLOR_PAIR(3));
}