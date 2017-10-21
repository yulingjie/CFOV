#include "TileMap.h"
#include <stdio.h>
void TileMap::CreateMap()
{
	int i, j; /* coordinate counters */

	/* initialize the map to ground (GROUND) */
	for (j = 0; j != MAP_HEIGHT; ++j) {
		for (i = 0; i != MAP_WIDTH; ++i) {
			_Map[i][j].ch = GROUND;
			_Map[i][j].seen = false;
			_Map[i][j].remembered = false;
		}
	}

	/* draw some fairly arbitrary walls */
	for (i = 10; i != 20; ++i)
		_Map[i][10].ch = WALL;
	for (i = 10; i != 40; ++i)
		_Map[i][20].ch = WALL;

	for (j = 10; j != 21; ++j)
		_Map[10][j].ch = WALL;
	for (i = 10; i != 21; ++i)
		_Map[40][i].ch = WALL;

	for (i = 10; i != 21; ++i)
		_Map[11][i].ch = WALL;
	for (i = 10; i != 21; ++i)
		_Map[12][i].ch = WALL;

	for (i = 20; i != 25; ++i)
		_Map[30][i].ch = WALL;

	for (j = 15; j != 70; ++j)
		_Map[j][30].ch = WALL;
}
Tile* TileMap::GetTile(int x, int y)
{
	if (ValidTile(x, y))
	{
		return &_Map[x][y];
	}
	else
	{
		return NULL;
	}
}
bool TileMap::ValidTile(int x, int y)
{
	return (((unsigned)x < MAP_WIDTH) && ((unsigned)y < MAP_HEIGHT));
}
