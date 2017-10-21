#pragma once

class Size
{
public:
	Size(int width, int height);
	int Width() const { return _width; }
	int Height() const { return _height; }
private:
	int _width;
	int _height;
};