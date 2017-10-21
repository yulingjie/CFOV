#pragma once
#include "Visibility.h"
#include "Size.h"
#include "Common.h"

class RayCastVisibility: public Visibility
{
public:
	RayCastVisibility(const Size mapSize,
		const Func_I_I_B blocksLight,
		const Func_I_I_V setVisible, 
		const Func_I_I_I_I_I getDistance);
	void Compute(LevelPoint origin, int rangeLimit);
private:
	void TraceLine(LevelPoint origin, int x2, int y2, int rangeLimit);
	
private: 
	const Size _MapSize;
	const Func_I_I_B _BlocksLight;
	const Func_I_I_I_I_I _GetDistance;
	const Func_I_I_V _SetVisible;
};