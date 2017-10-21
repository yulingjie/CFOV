#include "Rectangle.h"
#include "Common.h"

Rectangle::Rectangle(int x, int y, int width, int height)
	:_x(x),
	_y(y),
	_width(width),
	_height(height)
{

}

void Rectangle::Intersect(const Rectangle& rhs)
{
	if ((rhs._x > _x + _width || _x > rhs._x + rhs._width)
		|| (rhs._y > _y + _height || _y > rhs._y + rhs._height))
	{
		_x = _y = _width = _height = 0;
	}
	_x = Max(_x, rhs._x);
	_y = Max(_y, rhs._y);
	_width = Min(_x + _width, rhs._x + rhs._width) - _x;
	_height = Min(_y + _height, rhs._y + rhs._height) - _y;
}