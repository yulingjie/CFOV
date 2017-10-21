#include "Common.h"

int Abs(int x)
{
	return x < 0 ? -x : x;
}
int Sign(int x)
{
	return x >= 0 ? 1 : -1;
}

void Swap(int& a, int& b)
{
	int x = a;
	a = b;
	b = x;
}
int Max(int a, int b)
{
	return a > b ? a : b;
}

int Min(int a, int b)
{
	return a > b ? b : a;
}