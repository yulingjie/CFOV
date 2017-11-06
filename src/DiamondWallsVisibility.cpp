#include <cstdio>
#include <cstdlib>
#include <plog/Log.h>
#include "DiamondWallsVisibility.h"

DiamondWallsVisibility::DiamondWallsVisibility(Func_I_I_B blocksLight, Func_I_I_V setVisible, Func_I_I_I getDistance)
	:_blocksLight(blocksLight),
	_setVisible(setVisible),
	_getDistance(getDistance)
{
	_results = NULL;

}

DiamondWallsVisibility::~DiamondWallsVisibility()
{
	DisposeResults();
}
void DiamondWallsVisibility::DisposeResults()
{
	LOGD << "ClearResults Begin";
	if (_results != NULL)
	{
		for (int i = 0; i != 2 *_rangeLimit + 1; ++i)
		{
			for (int j = 0; j != 2 * _rangeLimit + 1; ++j)
			{
				if (_results[i][j] != NULL)
				{
					delete _results[i][j];
				}
				_results[i][j] = NULL;
			}
		}
		for (int i = 0; i != 2 * _rangeLimit + 1; ++i)
		{
			if (_results[i] != NULL)
			{
				delete[] _results[i];
			}
			_results[i] = NULL;
		}
		delete[] _results;
	}
	while (!_pool.empty())
	{
		RayData* rd = _pool.front();
		_pool.pop();
		if (rd != NULL)
		{
			delete rd;
		}
	}
	LOGD << "ClearResults End";
}
void DiamondWallsVisibility::CreateResults()
{
	LOGD << "CreateResults Beg";
	_results = new RayData**[2 * _rangeLimit + 1];
	for (int i = 0; i != 2*_rangeLimit +1; ++i)
	{		
		_results[i] = new RayData*[2 * _rangeLimit + 1];
		for (int j = 0; j != 2 * _rangeLimit + 1; ++j)
		{
			_results[i][j] = NULL;
		}		
	}
	LOGD << "CreateResults End";
}
void DiamondWallsVisibility::Compute(LevelPoint origin, int rangeLimit)
{

	_origin = origin;
	_rangeLimit = rangeLimit;	
	if (_results == NULL)
	{
		CreateResults();
	}
	
	CastRays();	
	while (!_perimeter.empty())
	{
		_perimeter.pop();
	}	
	ClearResults();
	
}
void DiamondWallsVisibility::UnSpawn(RayData* rayData)
{
	if (rayData == NULL)
	{
		LOGD << "UnSpawn Error, rayData is NULL";
	}
	_pool.push(rayData);
}
DiamondWallsVisibility::RayData* DiamondWallsVisibility::Spawn()
{
	if (_pool.empty())
	{
		RayData* rd = new RayData();
		_pool.push(rd);
	}
	RayData* rayData = _pool.front();
	_pool.pop();
	rayData->Reset();
	return rayData;
}
void DiamondWallsVisibility::ClearResults()
{
	for (int i = 0; i != _rangeLimit * 2 + 1; ++i)
	{
		for (int j = 0; j != _rangeLimit * 2 + 1; ++j)
		{
			if (_results[i][j] != NULL)
			{
				UnSpawn(_results[i][j]);
			}
			_results[i][j] = NULL;
		}
	}
}
void DiamondWallsVisibility::CastRays()
{
	RayData* pZero = Spawn();
	pZero->xloc = 0;
	pZero->yloc = 0;
	ExpandPerimeterFrom(pZero);
	RayData* currentData;
	while (!_perimeter.empty())
	{
		currentData = _perimeter.front();
		_perimeter.pop();
		MergeInputs(currentData);

		if (!currentData->Obscure())
		{
			_setVisible(_origin.X() + currentData->xloc,
				_origin.Y() + currentData->yloc);
		}
		if (!currentData->ignore)
		{
			ExpandPerimeterFrom(currentData);
		}
	}
	if (pZero != NULL)
	{
		delete pZero;
		pZero = NULL;
	}
}

DiamondWallsVisibility::RayData* DiamondWallsVisibility::GetRayData(int x, int y)
{
	return _results[x + _rangeLimit][y+ _rangeLimit];	
}
void DiamondWallsVisibility::SetRayData(RayData* rayData)
{
	_results[rayData->xloc + _rangeLimit][rayData->yloc + _rangeLimit] = rayData;
}
void DiamondWallsVisibility::ExpandPerimeterFrom(const RayData* from)
{
	if (from == NULL) return;
	if (from->xloc >= 0)
	{
		RayData * rd = Spawn();
		rd->xloc = from->xloc + 1;
		rd->yloc = from->yloc;
		ProcessRay(rd, from);
	}
	if (from->xloc <= 0)
	{
		RayData* rd = Spawn();
		rd->xloc = from->xloc - 1;
		rd->yloc = from->yloc;
		ProcessRay(rd, from);
	}
	if (from->yloc >= 0)
	{
		RayData* rd = Spawn();
		rd->xloc = from->xloc;
		rd->yloc = from->yloc + 1;
		ProcessRay(rd, from);
	}
	if (from->yloc <= 0)
	{
		RayData* rd = Spawn();
		rd->xloc = from->xloc;
		rd->yloc = from->yloc - 1;
		ProcessRay(rd, from);
	}
	LOGD << "ExpandPerimeter End";
}
void DiamondWallsVisibility::ProcessRay(RayData* newRay, const RayData* inputRay)
{
	LOGD << "ProcessRay Beg";
	if (_rangeLimit < _getDistance(newRay->xloc ,newRay->yloc)) return;
	RayData* pRayData = NULL;
	if (GetRayData(newRay->xloc, newRay->yloc) != NULL)
	{
		pRayData = GetRayData(newRay->xloc, newRay->yloc);
		if (newRay != NULL)
		{
			UnSpawn(newRay);
			newRay = NULL;
		}
	}
	else
	{
		pRayData = newRay;
	}
	bool isXInput = (pRayData->yloc == inputRay->yloc);
	if (isXInput)
	{
		pRayData->xinput = inputRay;
	}
	else
	{
		pRayData->yinput = inputRay;
	}
	if (!pRayData->added)
	{
		_perimeter.push(pRayData);
		pRayData->added = true;
		SetRayData(pRayData);
	}
}
void DiamondWallsVisibility::MergeInputs(RayData* newRay)
{
	LOGD << "MergeInputs Beg";
	if (_blocksLight(newRay->xloc + _origin.X(), newRay->yloc + _origin.Y()))
	{
		int absXLoc = abs(newRay->xloc);
		int absYLoc = abs(newRay->yloc);
		newRay->xob = absXLoc;
		newRay->yob = absYLoc;
		newRay->xerr = newRay->xob;
		newRay->yerr = newRay->yob;
		return;
	}
	const RayData* xInput = newRay->xinput;
	const RayData* yInput = newRay->yinput;
	bool xInputNull = (xInput == NULL);
	bool yInputNull = (yInput == NULL);

	if (!xInputNull) ProcessXInput(newRay, xInput);
	if (!yInputNull) ProcessYInput(newRay, yInput);
	if (!xInputNull && !yInputNull)
	{
		LOGD << "XInput IsNULL YInput IsNULL";
	}
	LOGD << "Normal";
	if (xInputNull)
	{
		if (yInput->Obscure()) newRay->ignore = true;
		LOGD << "MergeInput end";
	}
	else if (yInputNull)
	{
		if (xInput->Obscure()) newRay->ignore = true;
		LOGD << "MergeInput end";
	}
	else
	{
		if (xInput->Obscure() && yInput->Obscure())
		{
			newRay->ignore = true;
			LOGD << "MergeInput end";
			return;
		}
	}
	
}
void DiamondWallsVisibility::ProcessXInput(RayData* newRay, const RayData* xInput)
{
	LOGD << "ProcessXInput Beg";
	if ((xInput->xob == 0) && (xInput->yob == 0)) return;
	if (xInput->xerr > 0)
	{
		if (newRay->xob == 0)
		{
			newRay->xerr = (xInput->xerr - xInput->yob);
			newRay->yerr = (xInput->yerr + xInput->yob);
			newRay->yob = xInput->yob;
			newRay->xob = xInput->xob;
		}
	}
	if (xInput->yerr <= 0)
	{
		if ((xInput->yob > 0) && (xInput->xerr > 0))
		{
			newRay->yerr = (xInput->yob + xInput->yerr);
			newRay->xerr = (xInput->xerr - xInput->yob);
			newRay->xob = xInput->xob;
			newRay->yob = xInput->yob;
		}
	}
}
void DiamondWallsVisibility::ProcessYInput(RayData* newRay, const RayData* yInput)
{
	LOGD << "ProcessYInput Beg";
	if (yInput == NULL)
	{
		LOGD << "Error YInput is NULL";
	}
	if ((yInput->xob == 0) && (yInput->yob == 0))
	{
		LOGD << "ProcessYInput End";
		return;
	}
	LOGD << "ProcessYInput Normal";
	if (yInput->yerr > 0)
	{
		if (newRay->yob == 0)
		{
			newRay->yerr = yInput->yerr - yInput->xob;
			newRay->xerr = yInput->xerr + yInput->xob;
			newRay->xob = yInput->xob;
			newRay->yob = yInput->yob;
		}
	}
	if (yInput->xerr <= 0)
	{
		if ((yInput->xob > 0) && (yInput->yerr > 0))
		{
			newRay->xerr = (yInput->xob + yInput->xerr);
			newRay->yerr = (yInput->yerr - yInput->xob);
			newRay->xob = yInput->xob;
			newRay->yob = yInput->yob;
		}
	}
	LOGD << "ProcessYInput End";
}
DiamondWallsVisibility::RayData::RayData(const RayData& rhs)
{
	xloc = rhs.xloc;
	yloc = rhs.yloc;
	xob = rhs.xob;
	yob = rhs.yob;
	xerr = rhs.xerr;
	yerr = rhs.yerr;
	xinput = rhs.xinput;
	yinput = rhs.yinput;
	added = rhs.added;
	ignore = rhs.ignore;
}

DiamondWallsVisibility::RayData::RayData(int x, int y)
	:xloc(x), yloc(y),
	xob(0), yob(0),
	xerr(0), yerr(0),
	xinput(NULL), yinput(NULL),
	added(false), ignore(false)
{
	
}
DiamondWallsVisibility::RayData::RayData()
	:xloc(0),yloc(0),
	xob(0), yob(0),
	xerr(0), yerr(0),
	xinput(NULL), yinput(NULL),
	added(false), ignore(false)
{

}
DiamondWallsVisibility::RayData& DiamondWallsVisibility::RayData::operator=(const RayData& rhs)
{
	xloc = rhs.xloc;
	yloc = rhs.yloc;
	xerr = rhs.xerr;
	yerr = rhs.yerr;
	xinput = rhs.xinput;
	yinput = rhs.yinput;
	added = rhs.added;
	ignore = rhs.ignore;
	return *this;
}

bool DiamondWallsVisibility::RayData::Obscure() const
{
	return ((xerr > 0) && (xerr <= xob))
		|| ((yerr > 0) && yerr <= yob);
}
void DiamondWallsVisibility::RayData::Reset()
{
	xloc = yloc = xob = yob = 0;
	xinput = yinput = NULL;
	added = false;
	ignore = false;
}