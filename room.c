#include <curses.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "dungeon.h"

// fungsi gambar arena
void create_room(int coordinate_x, int coordinate_y, int room_width, int room_height, game_map *map)
{
    if (!map || !map->tiles)
        return;

    // inisialisasi semua tile sebagai VOID
    for (int y = 0; y < map->height_tile_map; y++)
    {
        for (int x = 0; x < map->width_tile_map; x++)
        {
            map->tiles[y][x] = TILE_VOID;
        }
    }

    // posisi room
    int room_x = ((map->width_tile_map - room_width) / 2) + coordinate_x;
    int room_y = ((map->height_tile_map - room_height) / 2) + coordinate_y;

    // simpan data posisi room
    map->room_x = room_x;
    map->room_y = room_y;
    map->room_w = room_width;
    map->room_h = room_height;

    // set room bounds
    init_room_bounds(map);

    // buat floor
    for (int y = room_y; y < room_y + room_height; y++)
    {
        for (int x = room_x; x < room_x + room_width; x++)
        {
            if (x >= 0 && x < map->width_tile_map && y >= 0 && y < map->height_tile_map)
            {
                map->tiles[y][x] = TILE_FLOOR;
            }
        }
    }

    // gambaer wall
    for (int x = room_x; x < room_x + room_width; x++)
    {
        // atas
        if (room_y - 1 >= 0)
            map->tiles[room_y - 1][x] = TILE_WALL;
        // bawah
        if (room_y + room_height < map->height_tile_map)
            map->tiles[room_y + room_height][x] = TILE_WALL;
    }

    for (int y = room_y; y < room_y + room_height; y++)
    {
        // kiri
        if (room_x - 1 >= 0)
            map->tiles[y][room_x - 1] = TILE_WALL;
        // kanan
        if (room_x + room_width < map->width_tile_map)
            map->tiles[y][room_x + room_width] = TILE_WALL;
    }
}

// wall yang rencana buat obstacle
// wall function
void wall(game_map *map, int coordinate_x, int coordinate_y, int room_width, int room_height)
{
    // posisi room
    int room_x = ((map->width_tile_map - room_width) / 2) + coordinate_x;
    int room_y = ((map->height_tile_map - room_height) / 2) + coordinate_y;

    // simpan data posisi room
    map->room_x = room_x;
    map->room_y = room_y;
    map->room_w = room_width;
    map->room_h = room_height;

    // set room bounds
    init_room_bounds(map);

    // buat floor
    for (int y = room_y; y < room_y + room_height; y++)
    {
        for (int x = room_x; x < room_x + room_width; x++)
        {
            if (x >= 0 && x < map->width_tile_map && y >= 0 && y < map->height_tile_map)
            {
                map->tiles[y][x] = TILE_FLOOR;
            }
        }
    }

    // gambaer wall
    for (int x = room_x; x < room_x + room_width; x++)
    {
        // atas
        if (room_y - 1 >= 0)
            map->tiles[room_y - 1][x] = TILE_WALL;
        // bawah
        if (room_y + room_height < map->height_tile_map)
            map->tiles[room_y + room_height][x] = TILE_WALL;
    }

    for (int y = room_y; y < room_y + room_height; y++)
    {
        // kiri
        if (room_x - 1 >= 0)
            map->tiles[y][room_x - 1] = TILE_WALL;
        // kanan
        if (room_x + room_width < map->width_tile_map)
            map->tiles[y][room_x + room_width] = TILE_WALL;
    }
}

// fungsi gambar pintu
void door(DoorFacing facing, int door_size, int open, game_map *map)
{
    if (!map)
        return;

    int start_x = 0;
    int start_y = 0;
    int tile = open ? TILE_DOOR_OPEN : TILE_DOOR_CLOSED;

    switch (facing)
    {
    case DOOR_LEFT:
        start_x = map->room_x - 1;
        start_y = map->room_y + (map->room_h - door_size) / 2;
        for (int i = 0; i < door_size; i++)
            map->tiles[start_y + i][start_x] = tile;
        break;

    case DOOR_RIGHT:
        start_x = map->room_x + map->room_w;
        start_y = map->room_y + (map->room_h - door_size) / 2;
        for (int i = 0; i < door_size; i++)
            map->tiles[start_y + i][start_x] = tile;
        break;

    case DOOR_UP:
        start_y = map->room_y - 1;
        start_x = map->room_x + (map->room_w - door_size) / 2;
        for (int i = 0; i < door_size; i++)
            map->tiles[start_y][start_x + i] = tile;
        break;

    case DOOR_DOWN:
        start_y = map->room_y + map->room_h;
        start_x = map->room_x + (map->room_w - door_size) / 2;
        for (int i = 0; i < door_size; i++)
            map->tiles[start_y][start_x + i] = tile;
        break;
    }
}

// fungsi gambar pintu di semua sisi arena
void door_4_side(int type_dungeon_size, int type_door, game_map *map)
{

    // gambar pintu untuk arena yang ukurannya ganjil
    if (type_dungeon_size == DOOR_ODD)
    {
        switch (type_door)
        {
        case DOOR_CLOSED:

            door(DOOR_LEFT, 3, DOOR_CLOSED, map);  // kiri
            door(DOOR_UP, 3, DOOR_CLOSED, map);    // atas
            door(DOOR_RIGHT, 3, DOOR_CLOSED, map); // kanan
            door(DOOR_DOWN, 3, DOOR_CLOSED, map);  // bawah
            break;

        case DOOR_OPEN:
            door(DOOR_LEFT, 3, DOOR_OPEN, map);  // kiri
            door(DOOR_UP, 3, DOOR_OPEN, map);    // atas
            door(DOOR_RIGHT, 3, DOOR_OPEN, map); // kanan
            door(DOOR_DOWN, 3, DOOR_OPEN, map);  // bawah
            break;
            break;
        }
    }

    // gambar pintu untuk arena yang ukurannya genap
    if (type_dungeon_size == DOOR_EVEN)
    {
        switch (type_door)
        {
        case DOOR_CLOSED:
            door(DOOR_LEFT, 4, DOOR_CLOSED, map);  // kiri
            door(DOOR_UP, 4, DOOR_CLOSED, map);    // atas
            door(DOOR_RIGHT, 4, DOOR_CLOSED, map); // kanan
            door(DOOR_DOWN, 4, DOOR_CLOSED, map);  // bawah
            break;
        case DOOR_OPEN:
            door(DOOR_LEFT, 4, DOOR_OPEN, map);  // kiri
            door(DOOR_UP, 4, DOOR_OPEN, map);    // atas
            door(DOOR_RIGHT, 4, DOOR_OPEN, map); // kanan
            door(DOOR_DOWN, 4, DOOR_OPEN, map);  // bawah
            break;
        }
    }
}

//============================== template room function ==============================//

// room 1
void room_1(game_map *map)
{
    create_room(0, 0, 20, 20, map);
    door_4_side(DOOR_EVEN, DOOR_OPEN, map);
}

// room 2
void room_2(game_map *map)
{
    create_room(0, 0, 20, 20, map);
    door_4_side(DOOR_EVEN, DOOR_CLOSED, map);
}

// room 3
void room_3(game_map *map)
{
    create_room(0, 0, 25, 25, map);
    door_4_side(DOOR_ODD, DOOR_OPEN, map);
}

// room 4
void room_4(game_map *map)
{
    create_room(0, 0, 25, 25, map);
    door_4_side(DOOR_ODD, DOOR_CLOSED, map);
}