#pragma once
#include "Common.h"
#include "Tile.h"
class TileMap
{
public:
	void CreateMap();
	Tile* GetTile(int x, int y);
	int GetPlayerX() { return _PlayerX; }
	int GetPlayerY() { return _PlayerY; }
	void SetPlayerX(int x) { _PlayerX = x; }
	void SetPlayerY(int y) { _PlayerY = y; }
	bool ValidTile(int x, int y);
private:	
	
	
private:	
	Tile _Map[MAP_WIDTH][MAP_HEIGHT];
	int _PlayerX;
	int _PlayerY;
};