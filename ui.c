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

void ui_game(game_map *map)
{
    if (!map)
        return;

    int allign_left = 2 * 55;
    WINDOW *setting_border;
    WINDOW *gameplay_border;

    size_of_screen = create_box(0, 0, 75, 40);
    gameplay_border = create_box_center(0, 2, 36, 36);
    setting_border = create_box(allign_left + 9, 5, 11, 29);

    if (map && map->arena_window == NULL)
    {
        // Hitung posisi dan ukuran arena window
        int arena_x = (screenwidth / 2) - (map->width_tile_map * 2 / 2);
        int arena_y = 4; // Di bawah gameplay_border top border

        map->arena_window = newwin(
            map->height_tile_map + 2,      // +2 untuk border
            (map->width_tile_map * 2) + 2, // *2 untuk karakter lebar, +2 untuk border
            arena_y,
            arena_x);
        keypad(map->arena_window, TRUE);  // Enable keypad untuk input
        nodelay(map->arena_window, TRUE); // Non-blocking input
    }

    PANEL *size_of_screen_p = new_panel(size_of_screen);
    PANEL *gameplay_border_p = new_panel(gameplay_border);
    PANEL *setting_border_p = new_panel(setting_border);
    PANEL *arena_p = NULL;

    if (map && map->arena_window)
    {
        arena_p = new_panel(map->arena_window);
    }

    // PERBAIKAN: Urutan panel dari bawah ke atas
    bottom_panel(size_of_screen_p);
    set_panel_userptr(size_of_screen_p, NULL);

    top_panel(gameplay_border_p);
    set_panel_userptr(gameplay_border_p, NULL);

    if (arena_p)
    {
        top_panel(arena_p);
        set_panel_userptr(arena_p, map);
    }

    top_panel(setting_border_p);
    set_panel_userptr(setting_border_p, NULL);

    update_panels();
    doupdate();

    // PERBAIKAN: Refresh semua windows
    wrefresh(size_of_screen);
    wrefresh(gameplay_border);
    wrefresh(setting_border);
    if (map && map->arena_window)
    {
        wrefresh(map->arena_window);
    }
}