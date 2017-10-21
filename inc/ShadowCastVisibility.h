#pragma once
#include "Visibility.h"
#include "Common.h"

class ShadowCastVisibility : public Visibility
{
private:
	struct Slope 
	{
		int X;
		int Y;
		Slope(int y, int x)
			:X(x), Y(y)
		{

		}
	};
public:
	ShadowCastVisibility(const Func_I_I_B blocksLight, const Func_I_I_V setVisible, const Func_I_I_I_I_I getDistance);
	void Compute(LevelPoint origin, int rangeLimit);
private:
	void ComputeInternal(unsigned int octant, LevelPoint origin, int rangeLimit, int x, Slope top, Slope bottom);
private:
	Func_I_I_B _BlocksLight;
	Func_I_I_I_I_I _GetDistance;
	Func_I_I_V _SetVisible;

};