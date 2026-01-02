#include <curses.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "dungeon.h"
#include <panel.h>

WINDOW *size_of_screen;

// fungsi gambar kotak dengan posisi center
WINDOW *create_box_center(int x, int y, int size_x, int size_y)
{
    WINDOW *boxs = newwin(size_y, (size_x * 2), y, (((screenwidth / 2) - size_x) + x));
    box(boxs, 0, 0);
    return boxs;
}

// fungsi gambar kotak
WINDOW *create_box(int x, int y, int size_x, int size_y)
{
    WINDOW *boxs = newwin(size_y, (size_x * 2), y, x);
    box(boxs, 0, 0);
    return boxs;
}

// fungsi posisi dari arena
void location_of_arena(int x, int y, game_map *map)
{
    if (map && map->arena_window == NULL)
    {
        // potition for arena windows
        int arena_x = ((screenwidth / 2)) - (map->width_tile_map);
        int arena_y = 3;

        int x_final_arena = arena_x + x;
        int y_final_arena = arena_y + y;

        map->arena_window = newwin(map->height_tile_map, (map->width_tile_map * 2), y_final_arena, x_final_arena);
        keypad(map->arena_window, TRUE);
        nodelay(map->arena_window, TRUE);
    }
}

// fungsi ui selama game
void ui_game(game_map *map)
{
    if (!map)
        return;

    //inisialisasi window untuk panel
    int allign_left = 2 * 55;
    WINDOW *setting_border;
    WINDOW *gameplay_border;

    // desain ukuran box
    size_of_screen = create_box(0, 0, 75, 40);
    gameplay_border = create_box_center(0, 2, 36, 36);
    setting_border = create_box(allign_left + 9, 5, 11, 29);

    // inisialisasi panel handling
    PANEL *size_of_screen_p = new_panel(size_of_screen);
    PANEL *gameplay_border_p = new_panel(gameplay_border);
    PANEL *setting_border_p = new_panel(setting_border);
    PANEL *arena_p = NULL;
    if (map && map->arena_window)
    {
        arena_p = new_panel(map->arena_window); // panel arena di dalam panel gameplay
    }

    location_of_arena(0, 0, map);

    // urutan panel
    bottom_panel(size_of_screen_p); // panel paling luar
    set_panel_userptr(size_of_screen_p, NULL);

    top_panel(gameplay_border_p); // panel gameplay di tengah
    set_panel_userptr(gameplay_border_p, NULL);

    top_panel(setting_border_p); // panel setting di kanan
    set_panel_userptr(setting_border_p, NULL);

    update_panels();
    doupdate();

    // refresh semua windows
    wrefresh(size_of_screen);
    wrefresh(gameplay_border);
    wrefresh(setting_border);
    if (map && map->arena_window)
    {
        wrefresh(map->arena_window);
    }
}