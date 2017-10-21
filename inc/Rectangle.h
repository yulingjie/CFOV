#pragma once

class Rectangle
{
public:
	Rectangle(int x, int y, int width, int height);
	int Left() const { return _x; } 
	int Right() const { return _x + _width; }
	int Top() const { return _y; } const
	int Bottom() const { return _y + _height; } 

	void Intersect(const Rectangle& rhs);
private:
	int _x;
	int _y;
	int _width;
	int _height;
};