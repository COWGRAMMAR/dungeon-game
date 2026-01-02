#include <curses.h>
#include <windows.h>
#include <locale.h>
#include "dungeon.h"
#include <string.h>
#include <stdlib.h>

vec2 dir = {0, 0};
tile_properties tile_props[TILE_COUNT];

// inisialisasi tile properties
void init_tile_properties()
{
    // tile floor properti
    tile_props[TILE_FLOOR].type = TILE_FLOOR;
    tile_props[TILE_FLOOR].is_walkable = true;
    tile_props[TILE_FLOOR].is_transparent = true;
    tile_props[TILE_FLOOR].display_char = ' ';
    tile_props[TILE_FLOOR].color_pair = 1;

    // tile wall properti
    tile_props[TILE_WALL].type = TILE_WALL;
    tile_props[TILE_WALL].is_walkable = false;
    tile_props[TILE_WALL].is_transparent = false;
    tile_props[TILE_WALL].display_char = '#';
    tile_props[TILE_WALL].color_pair = 2;

    // tile close door properti
    tile_props[TILE_DOOR_CLOSED].type = TILE_DOOR_CLOSED;
    tile_props[TILE_DOOR_CLOSED].is_walkable = false;
    tile_props[TILE_DOOR_CLOSED].is_transparent = false;
    tile_props[TILE_DOOR_CLOSED].display_char = '+';
    tile_props[TILE_DOOR_CLOSED].color_pair = 3;

    // tile open door properti
    tile_props[TILE_DOOR_OPEN].type = TILE_DOOR_OPEN;
    tile_props[TILE_DOOR_OPEN].is_walkable = true;
    tile_props[TILE_DOOR_OPEN].is_transparent = true;
    tile_props[TILE_DOOR_OPEN].display_char = '+';
    tile_props[TILE_DOOR_OPEN].color_pair = 4;

    // tile void properti
    tile_props[TILE_VOID].type = TILE_VOID;
    tile_props[TILE_VOID].is_walkable = false;
    tile_props[TILE_VOID].is_transparent = false;
    tile_props[TILE_VOID].display_char = 'Q';
    tile_props[TILE_VOID].color_pair = 0;
}

// inisialisasi batas map
void init_room_bounds(game_map *map)
{
    map->active_room.x = map->room_x;
    map->active_room.y = map->room_y;
    map->active_room.width = map->room_w;
    map->active_room.height = map->room_h;

    // letak spawn player ditengah
    map->player_dir.x = map->room_x + (map->room_w / 2);
    map->player_dir.y = map->room_y + (map->room_h / 2);
}

// fungsi membuat batas map
game_map *create_map(int width_tile_map, int height_tile_map)
{
    game_map *map = malloc(sizeof(game_map));
    map->width_tile_map = width_tile_map;
    map->height_tile_map = height_tile_map;
    map->player_dir = (vec2){1, 1};
    map->is_running = true;
    map->arena_window = NULL;

    // alokasi array 2d
    map->tiles = malloc(height_tile_map * sizeof(tile_type *));
    for (int y = 0; y < height_tile_map; y++)
    {
        map->tiles[y] = malloc(width_tile_map * sizeof(tile_type));
    }

    return map;
}

// fungsi arena yang tersedia untuk berjalan
bool is_walkable_map(const game_map *map, int x, int y)
{
    // cek batas terlebih dahulu
    if (x < 0 || x >= map->width_tile_map || y < 0 || y >= map->height_tile_map)
    {
        return false;
    }

    // ambil tile type terlebih dahulu
    tile_type tile = map->tiles[y][x];

    // cek void tile
    if (tile == TILE_VOID)
    {
        return false;
    }

    // cek wall tile
    if (tile == TILE_WALL)
    {
        return false;
    }

    // jika pintu = TILE_DOOR_OPEN maka karakter boleh melangkah
    if (tile == TILE_DOOR_OPEN)
    {
        return true;
    }
    if (tile == TILE_DOOR_CLOSED)
    {
        return false;
    }

    // cek apakah berada di area yang aktif
    bool is_in_floor =
        (x >= map->active_room.x &&
         x < map->active_room.x + map->active_room.width &&
         y >= map->active_room.y &&
         y < map->active_room.y + map->active_room.height);

    // jika berada di floor area. maka ikuti properti
    if (is_in_floor)
    {
        return tile_props[tile].is_walkable;
    }

    // jika diluar floor area maka tidak bisa berjalan
    return false;
}

// fungsi move player
void map_move_player(game_map *map, vec2 direction)
{
    int new_x = map->player_dir.x + direction.x;
    int new_y = map->player_dir.y + direction.y;

    // cek collision berdasarkan tile properties
    if (is_walkable_map(map, new_x, new_y))
    {
        tile_type target_tile = map->tiles[new_y][new_x];

        // fungsi untuk handle tile khusus
        switch (target_tile)
        {
        case TILE_DOOR_CLOSED:
            map->tiles[new_y][new_x] = TILE_DOOR_OPEN;
            map->player_dir.x = new_x;
            map->player_dir.y = new_y;
            break;

        case TILE_DOOR_OPEN:
        case TILE_FLOOR:
        default:
            map->player_dir.x = new_x;
            map->player_dir.y = new_y;
            break;
        }
    }
}

// fungsi input movement dari user
int input_movement_player(game_map *map)
{
    if (!map || !map->arena_window)
        return 0;

    int pressed = wgetch(map->arena_window);
    vec2 dir = {0, 0};

    // input kekiri
    if (pressed == KEY_LEFT || pressed == 'a')
    {
        dir.x = -1;
        dir.y = 0;
    }

    // input kekanan
    else if (pressed == KEY_RIGHT || pressed == 'd')
    {
        dir.x = 1;
        dir.y = 0;
    }

    // input keatas
    else if (pressed == KEY_UP || pressed == 'w')
    {
        dir.x = 0;
        dir.y = -1;
    }

    // input kebawah
    else if (pressed == KEY_DOWN || pressed == 's')
    {
        dir.x = 0;
        dir.y = 1;
    }

    // input keluar
    else if (pressed == 'q')
    {
        map->is_running = false;
        return 1;
    }

    // ketika tidak ada input
    else if (pressed == ERR)
    {
        return 0;
    }

    // cek kondisi status posisi player saat ini
    if (dir.x != 0 || dir.y != 0)
    {
        map_move_player(map, dir);
    }

    return 0;
}

// fungsi update player
void update_player(game_map *map, DWORD now)
{
    if (!map)
        return;

    static DWORD last_move_time = 0;
    now = GetTickCount();

    if ((now - last_move_time) >= DelayInput)
    {
        last_move_time = now;
    }
}

// fungsi menggambar seluruh arena beserta isinya
void draw(game_map *map)
{
    if (!map || !map->arena_window)
        return;

    werase(map->arena_window);

    // fungsi gambar wall
    for (int y = 0; y < map->height_tile_map; y++)
    {
        for (int x = 0; x < map->width_tile_map; x++)
        {
            tile_type tile = map->tiles[y][x];
            tile_properties *props = &tile_props[tile];

            wattron(map->arena_window, COLOR_PAIR(props->color_pair));
            mvwaddch(map->arena_window, y + 1, (x * 2) + 1, props->display_char);
            wattroff(map->arena_window, COLOR_PAIR(props->color_pair));
        }
    }

    // fungsi gambar player
    wattron(map->arena_window, COLOR_PAIR(4) | A_BOLD);
    mvwaddch(map->arena_window, map->player_dir.y + 1, (map->player_dir.x * 2) + 1, 'O'); // Karakter player dari kode Anda
    wattroff(map->arena_window, COLOR_PAIR(4) | A_BOLD);

    // fungsi gambar debugging
    /*mvwprintw(map->arena_window, 0, 1, "Player: (%d,%d) Room: (%d,%d)",
              map->player_dir.x, map->player_dir.y,
              map->room_x, map->room_y);*/

    wrefresh(map->arena_window);
}

// fungsi desain 1 map (prototipe)
void create_simple_arena(game_map *map)
{
    if (!map || !map->tiles)
        return;

    int mid_y = map->height_tile_map / 2;
    int mid_x = map->width_tile_map / 2;

    // Set player position di tengah
    map->player_dir = (vec2){mid_x, mid_y};

    // desain mapnya
    create_room(map, -5, -2, 20, 20);
    door_4_side(2, 0, map);
    door(DOOR_LEFT, 4, 1, map);
}

// fungsi loop game utama
int game(game_state *state)
{
    if (!state || !state->current_map)
        return 1;

    ui_game(state->current_map);
    create_simple_arena(state->current_map);

    // game loop utama
    while (state->is_running && state->current_map->is_running)
    {
        DWORD now = GetTickCount();

        // Input handling
        int event = input_movement_player(state->current_map);
        if (event == 1)
        {
            break;
        }

        // Update game state
        update_player(state->current_map, now);

        // Render
        draw(state->current_map);

        // Delay
        napms(GameTick);
    }

    return 0;
}

// fungsi menghapus map sampai data array arraynya
void map_destroy(game_map *map)
{
    if (!map)
        return;

    // hapus window jika ada
    if (map->arena_window)
    {
        delwin(map->arena_window);
    }

    // hapus array 2D tiles
    if (map->tiles)
    {
        for (int y = 0; y < map->height_tile_map; y++)
        {
            free(map->tiles[y]);
        }
        free(map->tiles);
    }

    free(map);
}