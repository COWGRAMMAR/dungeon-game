#include <curses.h>
#include <windows.h>
#include <locale.h>
#include "dungeon.h"

#define DelayInput 75

// arena size
int arena_size = 30;

vec2 dir = {0, 0};
vec2 player_dir = {0, 0};

bool is_running = true;
bool skip = false;

DWORD last_move_time = 0;

WINDOW *arena;

// collide detection
bool collide(vec2 a, vec2 b)
{
    if (a.x == b.x && a.y == b.y)
    {
        return true;
    }
    else
        return false;
}

// collide detection function
int collide_wall(int next_x, int next_y, int arena)
{
    int min_x = arena - arena;
    int max_x = arena - 2;
    int min_y = arena - arena;
    int max_y = arena - 3;

    return (next_x < min_x || next_x > max_x ||
            next_y < min_y || next_y > max_y);
}

// input movement function
int input_movement_player()
{

    int pressed = wgetch(win);

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

    // when no input
    else if (pressed == ERR)
    {
        return 0;
    }

    return 0;
}

// update for player movement
void update_player()
{
    DWORD now = GetTickCount();

    if ((now - last_move_time) >= DelayInput)
    {

        int next_x_player = player_dir.x + dir.x;
        int next_y_player = player_dir.y + dir.y;

        if (collide_wall(next_x_player, next_y_player, arena_size))
        {
            dir.x = 0;
            dir.y = 0;
            last_move_time = now;
            return;
        }

        if (dir.x != 0 || dir.y != 0)
        {
            player_dir.x += dir.x;
            player_dir.y += dir.y;

            dir.x = 0;
            dir.y = 0;

            last_move_time = now;
        }
    }
}

// initialization arena function
void init_arena()
{
    int render_x = arena_size;
    int render_y = arena_size;
    arena = create_arena_box(0, 3, render_y, render_x);
}

// draw character function
void draw()
{
    werase(arena);
    box(arena, 0, 0);
    mvwaddch(arena, (player_dir.y + 1), ((player_dir.x * 2) + 1), 'O');
    wrefresh(arena);
}

// loop game logic function
int game()
{
    init_arena();
    ui_game();
    while (is_running)
    {
        int event = input_movement_player();

        if (event == 1)
        {
            return 1;
        }

        if (skip == true)
        {
            skip = false;
            continue;
        }

        update_player();
        draw();
        napms(16);
    }

    return 0;
}
