#ifndef MATH_H
#define MATH_H

#include <cmath>
#include "Int2.h"
#include "Float2.h"
#include "Float4.h"
#include "List.h"

struct Ratio
{
	int numerator;
	int denominator;

	Ratio() = default;

	Ratio(int numerator, int denominator)
	{
		this->numerator = numerator;
		this->denominator = denominator;
	}
};

struct Ratio2
{
	Ratio x;
	Ratio y;

	Ratio2() = default;

	Ratio2(Ratio x, Ratio y)
	{
		this->x = x;
		this->y = y;
	}

	Ratio2(Int2 numerators, Int2 denominators)
	{
		this->x = Ratio(numerators.x, denominators.x);
		this->y = Ratio(numerators.y, denominators.y);
	}
};

float Min(float a, float b);
float Max(float a, float b);
float Clamp(float val, float min, float max);
float Mod(float a, float b);

#endif