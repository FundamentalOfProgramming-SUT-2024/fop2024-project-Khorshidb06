#ifndef MAKEMAP_H
#define MAKEMAP_H

typedef struct {
    int x; 
    int y; 
    int tool;
    int arz;
    int doorx;
    int doory;
    int room_number;
    char type;
    
} room;


typedef struct {
    int x;
    int y;
} point;

typedef struct 
{
    char mname;
    int x;
    int y;
    int lifee;
    int movement;

}monster;

typedef struct 
{
    char type;
    int being;
    int trap;
    monster monster;
    char model;
}newpoint;



void print_map();
void makemap();
void play_the_game();
void load_game();
void pregamemenu();

#endif