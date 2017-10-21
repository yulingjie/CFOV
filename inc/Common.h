#pragma once
#define MAP_WIDTH 80
#define MAP_HEIGHT 50
#define PLAYER_START_X 20
#define PLAYER_START_Y 5
#define PLAYER '@'
#define GROUND '.'
#define WALL '#'
#define REMEMBERED_COLOUR 1
#define PLAYER_COLOUR 2
#define SEEN_COLOUR 3
#define PLAYER_RANGELIMIT 20
typedef bool (*Func_I_I_B)(int, int);
typedef int(*Func_I_I_I_I_I)(int, int, int, int);
typedef void(*Func_I_I_V) (int, int);
typedef int(*Func_I_I_I)(int, int, int);
int Abs(int x);

int Sign(int x);

void Swap(int& a, int& b);

int Max(int a, int b);

int Min(int a, int b);
