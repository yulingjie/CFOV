#pragma once
class LevelPoint
{
public:
	LevelPoint(int x, int y)
		:_x(x), _y(y)
	{

	}
	LevelPoint()		
		:_x(0),_y(0)
	{
		
	}
	int X() const
	{
		return _x;
	}
	int Y() const
	{
		return _y;
	}
private:
	int _x;
	int _y;
};