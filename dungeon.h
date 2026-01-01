#ifndef DUNGEON_H
#define DUNGEON_H

#include <curses.h>
#include <panel.h>
#include <stdbool.h>
#include <windows.h>

//============================== global function prototype ==============================//

extern int screenwidth;  // size of terminal width
extern int screenheight; // size of terminal height

extern WINDOW *win;

// vector properties
typedef struct
{
    int x;
    int y;
} vec2;

// player properties
typedef struct
{
    int health;
    int level;

} player_info;

// enum for type tile
typedef enum
{
    TILE_FLOOR = 0,
    TILE_WALL,
    TILE_DOOR_CLOSED,
    TILE_DOOR_OPEN,
    TILE_COUNT
} tile_type;

// tile properties
typedef struct
{
    tile_type type;
    bool is_walkable;
    bool is_transparent;
    int display_char;
    int color_pair;
} tile_properties;

// room bounderies
typedef struct 
{
    int x, y;
    int width, height;
    bool is_active;
} room_bounds;


// structure map properties
typedef struct
{
    int width_tile_map;
    int height_tile_map;
    tile_type **tiles;
    vec2 player_dir;
    bool is_running;
    WINDOW *arena_window;
    room_bounds room;
    int room_x, room_y;        
    int room_w, room_h; 
} game_map;

// game state
typedef struct
{
    game_map *current_map;
    bool is_running;
    bool skip_frame;
    DWORD last_move_time;
} game_state;

#define DelayInput 75
#define GameTick 16

//============================== global function prototype ==============================//

//============================== ui function prototype ==============================//

extern WINDOW *create_arena_box(int x, int y, int size_x, int size_y);  // arena border
extern WINDOW *create_box_center(int x, int y, int size_x, int size_y); // universal border with allign center
extern WINDOW *create_box(int x, int y, int size_x, int size_y);        // universal border
void location_of_arena(int x, int y, game_map *map);
void ui_game(game_map *map); // ui for in game

//============================== ui function prototype ==============================//

//============================== init function prototype ==============================//

void set_console_char_size(short cols, short rows); // setup windows terminal size
void init();                                        // initialize terminal
void tester();                                      // tester function for setup window terminal

//============================== init function prototype ==============================//

//============================== logic function prototype ==============================//

extern WINDOW *arena;

void init_tile_properties();
game_map *create_map(int width_tile_map, int height_tile_map);
void destroy_map(game_map *map);
void render_map(const game_map *map);
bool is_walkable_map(const game_map *map, int x, int y);

int input_movement_player(game_map *map);     // input user
void update_player(game_map *map, DWORD now); // update player movement
void map_move_player(game_map *map, vec2 direction);
void init_arena(game_map *map);
void draw(game_map *map); // draw arena, player, etc
int game(game_state *state);
void map_destroy(game_map *map); // logic state game

//============================== logic function prototype ==============================//

//============================== room function prototype ==============================//
#define ROOM_SMALL   1
#define ROOM_MEDIUM  2
#define ROOM_LARGE   3
void wall(game_map *map);
void door(int type_dungeon_size, int type_door, int facing, game_map *map);
void door_4_side(int type_dungeon_size, int type_door, game_map *map);

//============================== room function prototype ==============================//
#endif