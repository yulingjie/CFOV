#pragma once

class LevelPoint;

class Visibility
{
public:
	virtual void Compute(LevelPoint origin, int rangeLimit) = 0;

};