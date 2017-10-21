#include "RayCastVisibility.h"
#include "LevelPoint.h"
#include "Rectangle.h"
RayCastVisibility::RayCastVisibility(const Size mapSize, const Func_I_I_B blocksLight, const Func_I_I_V setVisible, const Func_I_I_I_I_I getDistance)
	:_MapSize(mapSize),
	_BlocksLight(blocksLight),
	_SetVisible(setVisible),
	_GetDistance(getDistance)
{
	
}
void RayCastVisibility::Compute(LevelPoint origin, int rangeLimit)
{
	_SetVisible(origin.X(), origin.Y());
	if (rangeLimit != 0)
	{
		Rectangle area(0, 0, _MapSize.Width(), _MapSize.Height());
		if (rangeLimit >= 0)
		{
			Rectangle b(origin.X() - rangeLimit, origin.Y() - rangeLimit, rangeLimit * 2 + 1, rangeLimit * 2 + 1);
			area.Intersect(b);
		}
		for (int x = area.Left(); x < area.Right(); ++x)
		{
			TraceLine(origin, x, area.Top(), rangeLimit);
			TraceLine(origin, x, area.Bottom() - 1, rangeLimit);
		}
		for (int y = area.Top() + 1; y < area.Bottom() - 1; ++y)
		{
			TraceLine(origin, area.Left(), y, rangeLimit);
			TraceLine(origin, area.Right() - 1, y, rangeLimit);
		}
	}
}

void RayCastVisibility::TraceLine(LevelPoint origin, int x2, int y2, int rangeLimit)
{
	int xDiff = x2 - origin.X();
	int yDiff = y2 - origin.Y();
	int xLen = Abs(xDiff);
	int yLen = Abs(yDiff);
	int xInc = Sign(xDiff), yInc = Sign(yDiff) << 16, index = (origin.Y() << 16) + origin.X();
	if (xLen < yLen)
	{
		Swap(xLen, yLen);
		Swap(xInc, yInc);
	}
	int errorInc = yLen * 2, error = -xLen, errorReset = xLen * 2;
	while (--xLen >= 0)
	{
		index += xInc;
		error += errorInc;
		if (error > 0)
		{
			error -= errorReset;
			index += yInc;
		}
		int x = index & 0xFFFF, y = index >> 16;
		if (rangeLimit >= 0 && _GetDistance(origin.X(), origin.Y(), x, y) > rangeLimit) break;;
		_SetVisible(x, y);
		if (_BlocksLight(x, y)) break;;
	}
}