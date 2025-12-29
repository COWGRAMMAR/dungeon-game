#include <curses.h>
#include <windows.h>
#include <locale.h>
#include "dungeon.h"
#include <string.h>
#include <stdlib.h>

// arena size
int arena_size = 30;

vec2 dir = {0, 0};
// vec2 player_dir = {0, 0};

tile_properties tile_props[TILE_COUNT];



void init_tile_properties()
{
    // TILE_FLOOR - menggunakan '.' atau spasi
    tile_props[TILE_FLOOR].type = TILE_FLOOR;
    tile_props[TILE_FLOOR].is_walkable = true;
    tile_props[TILE_FLOOR].is_transparent = true;
    tile_props[TILE_FLOOR].display_char = ' ';
    tile_props[TILE_FLOOR].color_pair = 1;

    // TILE_WALL - menggunakan karakter dinding
    tile_props[TILE_WALL].type = TILE_WALL;
    tile_props[TILE_WALL].is_walkable = false;
    tile_props[TILE_WALL].is_transparent = false;
    tile_props[TILE_WALL].display_char = '#'; // Atau ACS_VLINE/ACS_HLINE
    tile_props[TILE_WALL].color_pair = 2;

    // TILE_DOOR_CLOSED - menggunakan '+' dari kode Anda (jika ada)
    tile_props[TILE_DOOR_CLOSED].type = TILE_DOOR_CLOSED;
    tile_props[TILE_DOOR_CLOSED].is_walkable = false;
    tile_props[TILE_DOOR_CLOSED].is_transparent = false;
    tile_props[TILE_DOOR_CLOSED].display_char = '+';
    tile_props[TILE_DOOR_CLOSED].color_pair = 3;
}

game_map *create_map(int width_tile_map, int height_tile_map)
{
    game_map *map = malloc(sizeof(game_map));
    map->width_tile_map = width_tile_map;
    map->height_tile_map = height_tile_map;
    map->player_dir = (vec2){1, 1};
    map->is_running = true;
    map->arena_window = NULL;

    // Alokasi array 2D untuk tiles
    map->tiles = malloc(height_tile_map * sizeof(tile_type *));
    for (int y = 0; y < height_tile_map; y++)
    {
        map->tiles[y] = malloc(width_tile_map * sizeof(tile_type));

        // Inisialisasi dengan wall
        for (int x = 0; x < width_tile_map; x++)
        {
            map->tiles[y][x] = TILE_WALL;
        }
    }

    return map;
}

bool is_walkable_map(const game_map *map, int x, int y)
{
    // Cek boundary (seperti collide_wall di kode Anda)
    if (x < 0 || x >= map->width_tile_map || y < 0 || y >= map->height_tile_map)
    {
        return false;
    }

    tile_type tile = map->tiles[y][x];
    return tile_props[tile].is_walkable;
}

// Move player dengan sistem yang lebih baik
void map_move_player(game_map *map, vec2 direction)
{
    int new_x = map->player_dir.x + direction.x;
    int new_y = map->player_dir.y + direction.y;

    // Cek collision berdasarkan tile properties
    if (is_walkable_map(map, new_x, new_y))
    {
        tile_type target_tile = map->tiles[new_y][new_x];

        // Handle tile khusus
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
            // Pindahkan player
            map->player_dir.x = new_x;
            map->player_dir.y = new_y;
            break;
        }
    }
}

// input movement function
int input_movement_player(game_map *map)
{
    if (!map || !map->arena_window)
        return 0;

    int pressed = wgetch(map->arena_window);
    vec2 dir = {0, 0};

    // input for left
    if (pressed == KEY_LEFT || pressed == 'a')
    {
        dir.x = -1;
        dir.y = 0;
    }

    // input for right
    else if (pressed == KEY_RIGHT || pressed == 'd')
    {
        dir.x = 1;
        dir.y = 0;
    }

    // input for up
    else if (pressed == KEY_UP || pressed == 'w')
    {
        dir.x = 0;
        dir.y = -1;
    }

    // input for down
    else if (pressed == KEY_DOWN || pressed == 's')
    {
        dir.x = 0;
        dir.y = 1;
    }

    else if (pressed == 'q')
    {
        map->is_running = false;
        return 1;
    }

    // when no input
    else if (pressed == ERR)
    {
        return 0;
    }

    if (dir.x != 0 || dir.y != 0)
    {
        map_move_player(map, dir);
    }

    return 0;
}

// update for player movement
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

// draw character function
void draw(game_map *map)
{
    if (!map || !map->arena_window)
        return;

    werase(map->arena_window);
    box(map->arena_window, 0, 0);

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

    wattron(map->arena_window, COLOR_PAIR(4) | A_BOLD);
    mvwaddch(map->arena_window, map->player_dir.y + 1, (map->player_dir.x * 2) + 1, 'O'); // Karakter player dari kode Anda
    wattroff(map->arena_window, COLOR_PAIR(4) | A_BOLD);

    wattron(map->arena_window, COLOR_PAIR(1));
    mvwprintw(map->arena_window, map->height_tile_map + 1, 1, "Pos: (%d, %d) | Use WASD/Arrows", map->player_dir.x, map->player_dir.y);
    wattroff(map->arena_window, COLOR_PAIR(1));

    wrefresh(map->arena_window);
}

// Membuat map sederhana seperti ruangan di kode Anda
void create_simple_arena(game_map *map)
{
    // Buat ruangan di tengah
    int room_width = map->width_tile_map - 4;
    int room_height = map->height_tile_map - 4;
    int room_start_x = 2;
    int room_start_y = 2;

    for (int y = room_start_y; y < room_start_y + room_height; y++)
    {
        for (int x = room_start_x; x < room_start_x + room_width; x++)
        {
            map->tiles[y][x] = TILE_FLOOR;
        }
    }

    // Tambahkan beberapa pintu
    map->tiles[room_start_y + room_height / 2][room_start_x - 1] = TILE_DOOR_CLOSED;
    map->tiles[room_start_y + room_height / 2][room_start_x + room_width] = TILE_DOOR_CLOSED;

    // Set player position
    map->player_dir = (vec2){room_start_x + 2, room_start_y + 2};
}

// loop game logic function
int game(game_state *state)
{
    if (!state || !state->current_map)
        return 1;

    // PERBAIKAN: Panggil ui_game SEBELUM game loop
    ui_game(state->current_map);

    // Buat peta sederhana
    create_simple_arena(state->current_map);

    // Game loop
    while (state->is_running && state->current_map->is_running)
    {
        DWORD now = GetTickCount();

        // Input handling
        int event = input_movement_player(state->current_map);
        if (event == 1)
        {
            break;
        }

        // PERBAIKAN: Hapus skip_frame logic jika tidak diperlukan
        if (state->skip_frame)
        {
            state->skip_frame = false;
            continue;
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

void map_destroy(game_map *map)
{
    if (!map)
        return;

    // Hapus window jika ada
    if (map->arena_window)
    {
        delwin(map->arena_window);
    }

    // Hapus array 2D tiles
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