#include<stdio.h>
#include "curses.h"
#include "Common.h"
#include "TileMap.h"
#include "Visibility.h"
#include "RayCastVisibility.h"
#include "ShadowCastVisibility.h"
#include "LevelPoint.h"
#include <plog/Log.h>
#include <math.h>

TileMap map;
Visibility* vs;
int init_curses()
{
	initscr();
	noecho();
	curs_set(0);
	if (has_colors())
	{
		start_color();
	}
	else
	{
		printf("Your system does not support color. This demo requires colors!");
		return 1;
	}
	init_pair(0, COLOR_BLACK, COLOR_BLACK);
	init_pair(REMEMBERED_COLOUR, COLOR_WHITE, COLOR_BLACK);
	init_pair(PLAYER_COLOUR, COLOR_GREEN, COLOR_BLACK);
	init_pair(SEEN_COLOUR, COLOR_BLUE, COLOR_BLACK);
	return 0;
}
void draw_map()
{
	for (int j = 0; j < MAP_HEIGHT; ++j)
	{
		for (int i = 0; i < MAP_WIDTH; ++i)
		{
			Tile* tile = map.GetTile(i, j);
			if (tile != NULL)
			{
				if (tile->seen)
				{
					mvaddch(j, i, tile->ch | COLOR_PAIR(SEEN_COLOUR));
				}
				else if (tile->remembered)
				{
					mvaddch(j, i, tile->ch | COLOR_PAIR(REMEMBERED_COLOUR));
				}
			}			
		}
	}
	mvaddch(map.GetPlayerY(), map.GetPlayerX(), PLAYER | COLOR_PAIR(PLAYER_COLOUR));
	refresh();
}
void exit_curse()
{
	endwin();
}
bool BlocksLight(int x, int y)
{
	Tile* pTile = map.GetTile(x, y);
	if (pTile != NULL)
	{
		return pTile->ch == WALL;
	}
	else
	{
		return true;
	}
	return false;
}
void SetVisible(int x, int y)
{
	Tile* pTile = map.GetTile(x, y);
	if (pTile != NULL)
	{
		pTile->seen = true;
	}
}
int GetDistance(int x1, int y1, int x2, int y2)
{
	int x = x2 - x1;
	int y = y2 - y1;
	int rt = x*x + y*y;
	return (int)sqrt(rt);
}
int GetDistance_I_I(int x, int y)
{
	return (int)sqrt(x*x + y*y);
}
void DoFov()
{
	// unseen everything
	for (int j = 0; j < MAP_HEIGHT; ++j)
	{
		for (int i = 0; i < MAP_WIDTH; ++i)
		{
			Tile* pTile = map.GetTile(i, j);
			if (pTile != NULL)
			{
				if (pTile->seen)
				{
					pTile->remembered = true;
				}
				pTile->seen = false;
			}
		}
	}

	LevelPoint p(map.GetPlayerX(), map.GetPlayerY());
	vs->Compute(p, PLAYER_RANGELIMIT);

}
Visibility* GetRayCastVisibility()
{
	Size sz(MAP_WIDTH,MAP_HEIGHT);
	return new RayCastVisibility(sz, BlocksLight, SetVisible, GetDistance);
}
Visibility* GetShadowRaycastVisibility()
{
	return new ShadowCastVisibility(BlocksLight, SetVisible, GetDistance);
}
void MovePlayer(int dx, int dy)
{
	int px = map.GetPlayerX();
	int py = map.GetPlayerY();
	if (map.ValidTile(px + dx, py + dy))
	{
		Tile *pTile = map.GetTile(px + dx, py + dy);
		if (pTile != NULL && pTile->ch != WALL)
		{
			px += dx;
			py += dy;
		}
	}
	map.SetPlayerX(px);
	map.SetPlayerY(py);
}
int main(int argc, char** argv)
{	
	plog::init(plog::debug, "test.txt");
	LOGD << "Hello Log!";
	map.CreateMap();
	vs = GetShadowRaycastVisibility();
	if (init_curses() != 0) return -1;
	int k;
	
	for (;;)
	{
		DoFov();
		draw_map();
		k = getch();

		switch (k)
		{
		case ('8'): MovePlayer(0, -1); break;
		case ('2'): MovePlayer(0, 1); break;
		case ('4'): MovePlayer(-1, 0); break;
		case ('6'): MovePlayer(1, 0); break;
		case ('7'): MovePlayer(-1, -1); break;
		case ('3'): MovePlayer(1, 1); break;
		case ('9'): MovePlayer(1, -1); break;
		case ('1'): MovePlayer(-1, 1); break;
		case ('q'):
		case ('Q'):
		case (27):
		{
			LOGD << "exit";
			goto end;
		}
		break;
		}

	}
end:
	if (vs != NULL)
	{
		delete vs;
		vs = NULL;
	}
	exit_curse();
}