#pragma once
#include "Visibility.h"
#include "Common.h"
#include "LevelPoint.h"
#include <queue>
using namespace std;
class DiamondWallsVisibility : public Visibility
{
	struct RayData
	{
		int xloc, yloc;
		int xob, yob;
		int xerr, yerr;
		const RayData *xinput, *yinput;
		bool added;
		bool ignore;
		RayData();
		RayData(int x, int y);
		RayData(const RayData& rhs);
		
		bool Obscure() const;
		void Reset();
		RayData& operator=(const RayData& rhs);
	};
public :
	DiamondWallsVisibility(Func_I_I_B blocksLight, Func_I_I_V setVisible, Func_I_I_I getDistance);
	~DiamondWallsVisibility();
	void Compute(LevelPoint origin, int rangeLimit);
private:
	//void ComputeInternal(unsigned int octant, LevelPoint origin, int rangeLimit, int x, Slope top, Slope bottom);
	void ProcessRay(RayData* newRay,const RayData* inputRay);
	void MergeInputs(RayData* newRay);
	void ProcessXInput(RayData* newRay, const RayData* xInput);
	void ProcessYInput(RayData* newRay, const RayData* yInput);
	void CastRays();
	void ExpandPerimeterFrom(const RayData* from);
	RayData* GetRayData(int x, int y);
	void SetRayData(RayData* rayData);	
	void DisposeResults();
	void CreateResults();
	void ClearResults();
	RayData* Spawn();
	void UnSpawn(RayData* rayData);
private:
	Func_I_I_B _blocksLight;
	Func_I_I_V _setVisible;
	Func_I_I_I _getDistance;	
	RayData** *_results;
	int _rangeLimit;		
	queue<RayData*> _perimeter;
	LevelPoint _origin;
	queue<RayData*> _pool;
};