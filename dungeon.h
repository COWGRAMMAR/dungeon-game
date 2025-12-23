#ifndef DUNGEON_H
#define DUNGEON_H

//============================== global function prototype ==============================//

typedef struct
{
    int x;
    int y;
} vec2;

typedef struct
{
    int health;
    int level;

} player_info;

extern int screenwidth;
extern int screenheight;

extern WINDOW *win;

//============================== global function prototype ==============================//

//============================== ui function prototype ==============================//

extern WINDOW *create_arena_box(int x, int y, int size_x, int size_y);
extern WINDOW *create_box_center(int x, int y, int size_x, int size_y);
extern WINDOW *create_box(int x, int y, int size_x, int size_y);

void ui_game();

//============================== ui function prototype ==============================//

//============================== init function prototype ==============================//

void set_console_char_size(short cols, short rows);
void init();
void tester();

//============================== init function prototype ==============================//

//============================== logic function prototype ==============================//

extern WINDOW *arena;

bool collide(vec2 a, vec2 b);
int input_movement_player();

void update_player();
void draw();
int game();

//============================== logic function prototype ==============================//

#endif