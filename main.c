#include <curses.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "dungeon.h"

int main(int argc, char const *argv[])
{
    // process user args
    if (argc == 1)
    {
    }
    else if (argc == 3)
    {
        if (!strcmp(argv[1], "-d"))
        {
            if (sscanf(argv[2], "%dx%d", &screenwidth, &screenheight) != 2)
            {
                printf("Usage: snake [options]\nOptions:\n -d [width]x[height]"
                       "\tdefine dimensions of the screen\n\nDefault dimensions are 59x25\n");
                exit(1);
            }
        }
    }
    else
    {
        printf("Usage: snake [options]\nOptions:\n -d [width]x[height]"
               "\tdefine dimensions of the screen\n\nDefault dimensions are 59x25\n");
        exit(1);
    }

    set_console_char_size(screenwidth, screenheight);
    init();
    init_tile_properties();

    game_state state = {
        .current_map = create_map(40, 30), // arena_size dari kode Anda = 30
        .is_running = true,
        .skip_frame = false,
        .last_move_time = 0,
        .arena_size = 20};

    // PERBAIKAN: Tampilkan pesan loading
    mvprintw(10, screenwidth / 2 - 10, "Loading Dungeon Game...");
    refresh();
    napms(1000); // Tunggu sebentar

    int result = game(&state);

    // Cleanup
    if (state.current_map)
    {
        map_destroy(state.current_map);
    }

    endwin();

    // game();
    printf("Game exited with code: %d\n", result);

    return result;
}