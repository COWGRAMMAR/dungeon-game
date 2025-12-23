#include <curses.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "dungeon.h"
#include <panel.h>

WINDOW *size_of_screen;

// create arena box function
WINDOW *create_arena_box(int x, int y, int size_x, int size_y)
{
    WINDOW *boxs = newwin(size_y, (size_x * 2) - 1, y, (((screenwidth / 2) - size_x) + x));
    box(boxs, 0, 0);
    return boxs;
}

// universal box with center
WINDOW *create_box_center(int x, int y, int size_x, int size_y)
{
    WINDOW *boxs = newwin(size_y, (size_x * 2), y, (((screenwidth / 2) - size_x) + x));
    box(boxs, 0, 0);
    return boxs;
}

// universal box without center
WINDOW *create_box(int x, int y, int size_x, int size_y)
{
    WINDOW *boxs = newwin(size_y, (size_x * 2), y, x);
    box(boxs, 0, 0);
    return boxs;
}

void ui_game()
{
    int allign_left = 2 * 55;
    WINDOW *setting_border;
    WINDOW *gameplay_border;

    size_of_screen = create_box(0, 0, 75, 40);
    gameplay_border = create_box_center(0, 2, 36, 36);
    setting_border = create_box(allign_left + 9, 5, 11, 29);

    PANEL *size_of_screen_p = new_panel(size_of_screen);
    PANEL *gameplay_border_p = new_panel(gameplay_border);
    PANEL *setting_border_p = new_panel(setting_border);

    bottom_panel(size_of_screen_p);
    top_panel(gameplay_border_p);
    top_panel(setting_border_p);

    update_panels();
    doupdate();
}