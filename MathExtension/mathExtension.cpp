#include "mathExtension.h"
#include <cmath>

float cotanf(float _x)
{
	return 1.0f / std::tanf(_x);
}

float clamp(float _x, float _min, float _max)
{
	float result = _x;
	if (_x < _min) result = _min;
	else if (_x > _max) result = _max;
	return result;
}

int clamp(int _x, int _min, int _max)
{
	int result = _x;
	if (_x < _min) result = _min;
	else if (_x > _max) result = _max;
	return result;
}
