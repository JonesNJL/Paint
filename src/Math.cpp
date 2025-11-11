#include "Math.hpp"

float Min(float a, float b)
{
	if (a < b) { return a; }
	else { return b; }
}

float Max(float a, float b)
{
	if (a > b) { return a; }
	else { return b; }
}

float Clamp(float val, float min, float max)
{
	if (val > max) { return max; }
	else if (val < min) { return min; }
	else { return val; }
}

float Mod(float a, float b)
{
	return a - (b * floor(a / b));
}

float Normalize(float val)
{
	return val / abs(val);
}