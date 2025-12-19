#ifndef DUNGEON_H
#define DUNGEON_H

void set_console_char_size(short cols, short rows);
void init();

extern int screenwidth;
extern int screenheight;

typedef struct 
{
 int x;
 int y;
} vec2;

typedef struct 
{
    int health;
    int level;

} player;


#endif 