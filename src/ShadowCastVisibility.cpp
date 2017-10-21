#include "ShadowCastVisibility.h"
#include "LevelPoint.h"
ShadowCastVisibility::ShadowCastVisibility(const Func_I_I_B blocksLight, const Func_I_I_V setVisible, const Func_I_I_I_I_I getDistance)
	:_BlocksLight(blocksLight),
	_SetVisible(setVisible),
	_GetDistance(getDistance)
{

}
void ShadowCastVisibility::Compute(LevelPoint origin, int rangeLimit)
{
	_SetVisible(origin.X(), origin.Y());
	for (unsigned int octant = 0; octant < 8; ++octant)
	{
		ComputeInternal(octant, origin, rangeLimit, 1, Slope(1, 1), Slope(0, 1));
	}
}
void ShadowCastVisibility::ComputeInternal(unsigned int octant, LevelPoint origin, int rangeLimit, int x, Slope top, Slope bottom)
{
	for (; (unsigned int)x <= (unsigned int)rangeLimit; ++x)
	{
		int topY = top.X == 1 ? x : ((x * 2 + 1)* top.Y + top.X - 1) / (top.X * 2);
		int bottomY = bottom.Y == 0 ? 0 : ((x * 2 - 1) * bottom.Y + bottom.X) / (bottom.X * 2);

		int wasOpaque = -1;
		for (int y = topY; y >= bottomY; --y)
		{
			int tx = origin.X(), ty = origin.Y();
			switch (octant)
			{
			case 0: tx += x; ty -= y; break;
			case 1: tx += y; ty -= x; break;
			case 2: tx -= y; ty -= x; break;
			case 3: tx -= x; ty -= y; break;
			case 4: tx -= x; ty += y; break;
			case 5: tx -= y; ty += x; break;
			case 6: tx += y; ty += x; break;
			case 7: tx += x; ty += y; break;
			default:
				break;
			}
			bool inRange = rangeLimit < 0 || _GetDistance(origin.X(), origin.Y(), tx, ty) <= rangeLimit;
			if (inRange) _SetVisible(tx, ty);

			bool isOpaque = !inRange || _BlocksLight(tx, ty);
			if (x != rangeLimit)
			{
				if (isOpaque)
				{
					if (wasOpaque == 0)
					{
						Slope newBottom(y * 2 + 1, x * 2 - 1);
						if (!inRange || y == bottomY) {
							bottom = newBottom;
							break;
						}
						else
						{
							ComputeInternal(octant, origin, rangeLimit, x + 1, top, newBottom);
						}
					}
					wasOpaque = 1;
				}
				else
				{
					if (wasOpaque > 0) top = Slope(y * 2 + 1, x * 2 + 1);
					wasOpaque = 0;
				}
			}
		}
		if (wasOpaque != 0) break;
	}
}