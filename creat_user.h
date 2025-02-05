#ifndef CREAT_USER_H
#define CREAT_USER_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    int x,y;
}Pos;

typedef struct {
    char username[50];
    char password[50];
    char email[100];
    int gold;
    int finished_games;
    int experience;
    int life;
    int food;
    int foods[4];
    char selah[6];
    int inroom;
    char weapon;
    int floor;
    int potion[6];
    int credit;
    char color;
    char potionn;
} User;

User user;

void creat_user();

#endif
