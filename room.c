#include <curses.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "dungeon.h"

// wall function
void wall(game_map *map)
{
    // Buat seluruh map jadi floor
    for (int y = 0; y < map->height_tile_map; y++)
    {
        for (int x = 0; x < map->width_tile_map; x++)
        {
            map->tiles[y][x] = TILE_FLOOR;
        }
    }

    // buat wall
    for (int x = 0; x < map->width_tile_map; x++)
    {
        map->tiles[0][x] = TILE_WALL;                        // top
        map->tiles[map->height_tile_map - 1][x] = TILE_WALL; // bottom
    }

    for (int y = 0; y < map->height_tile_map; y++)
    {
        map->tiles[y][0] = TILE_WALL;                       // left
        map->tiles[y][map->width_tile_map - 1] = TILE_WALL; // right
    }
}

// door function
void door(int type_dungeon_size, int type_door, int facing, game_map *map)
{

    int mid_y = map->height_tile_map / 2;
    int mid_x = map->width_tile_map / 2;

    if (type_dungeon_size == 1)
    {
        if (type_door == 1)
        {
            switch (facing)
            {
            case 1:
                // Pintu kiri
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[(mid_y - 1) + i][0] = TILE_DOOR_OPEN;
                }
                break;

            case 2:
                // pintu kanan
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[(mid_y - 1) + i][map->width_tile_map - 1] = TILE_DOOR_OPEN;
                }
                break;
                // pintu atas
            case 3:
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[0][(mid_x - 1) + i] = TILE_DOOR_OPEN;
                }
                break;

            case 4:
                // pintu bawah
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[map->height_tile_map - 1][(mid_x - 1) + i] = TILE_DOOR_OPEN;
                }
                break;
            }
        }

        if (type_door == 0)
        {
            switch (facing)
            {
            case 1:
                // Pintu kiri
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[(mid_y - 1) + i][0] = TILE_DOOR_CLOSED;
                }
                break;

            case 2:
                // pintu kanan
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[(mid_y - 1) + i][map->width_tile_map - 1] = TILE_DOOR_CLOSED;
                }
                break;
                // pintu atas
            case 3:
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[0][(mid_x - 1) + i] = TILE_DOOR_CLOSED;
                }
                break;

            case 4:
                // pintu bawah
                for (int i = 0; i < 3; i++)
                {
                    map->tiles[map->height_tile_map - 1][(mid_x - 1) + i] = TILE_DOOR_CLOSED;
                }
                break;
            }
        }
    }

    if (type_dungeon_size == 2)
    {
        if (type_door == 1)
        {
            switch (facing)
            {
            case 1:
                // Pintu kiri
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[(mid_y - 2) + i][0] = TILE_DOOR_OPEN;
                }
                break;

            case 2:
                // pintu kanan
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[(mid_y - 2) + i][map->width_tile_map - 1] = TILE_DOOR_OPEN;
                }
                break;
                // pintu atas
            case 3:
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[0][(mid_x - 2) + i] = TILE_DOOR_OPEN;
                }
                break;

            case 4:
                // pintu bawah
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[map->height_tile_map - 1][(mid_x - 2) + i] = TILE_DOOR_OPEN;
                }
                break;
            }
        }

        if (type_door == 0)
        {
            switch (facing)
            {
            case 1:
                // Pintu kiri
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[(mid_y - 2) + i][0] = TILE_DOOR_CLOSED;
                }
                break;

            case 2:
                // pintu kanan
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[(mid_y - 2) + i][map->width_tile_map - 1] = TILE_DOOR_CLOSED;
                }
                break;
                // pintu atas
            case 3:
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[0][(mid_x - 2) + i] = TILE_DOOR_CLOSED;
                }
                break;

            case 4:
                // pintu bawah
                for (int i = 0; i < 4; i++)
                {
                    map->tiles[map->height_tile_map - 1][(mid_x - 2) + i] = TILE_DOOR_CLOSED;
                }
                break;
            }
        }
    }
}

// door function 4 side
void door_4_side(int type_dungeon_size, int type_door, game_map *map)
{

    if (type_dungeon_size == 1)
    {
        switch (type_door)
        {
        case 0:
            door(1, 0, 1, map); // left
            door(1, 0, 2, map); // top
            door(1, 0, 3, map); // right
            door(1, 0, 4, map); // bottom
            break;

        case 1:
            door(1, 1, 1, map); // left
            door(1, 1, 2, map); // top
            door(1, 1, 3, map); // right
            door(1, 1, 4, map); // bottom
            break;
        }
    }

    if (type_dungeon_size == 2)
    {
        switch (type_door)
        {
        case 0:
            door(2, 0, 1, map); // left
            door(2, 0, 2, map); // top
            door(2, 0, 3, map); // right
            door(2, 0, 4, map); // bottom
            break;

        case 1:
            door(2, 1, 1, map); // left
            door(2, 1, 2, map); // top
            door(2, 1, 3, map); // right
            door(2, 1, 4, map); // bottom
            break;
        }
    }
}