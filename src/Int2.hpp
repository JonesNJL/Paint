#ifndef INT2_H
#define INT2_H

#include "Float2.hpp"

struct Int2
{
	int x = 0;
	int y = 0;

	Int2() = default;

	Int2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Int2 operator+ (const Int2& other)
	{
		Int2 result = Int2(this->x + other.x, this->y + other.y);
		return result;
	}

	Int2 operator- (const Int2& other)
	{
		Int2 result = Int2(this->x - other.x, this->y - other.y);
		return result;
	}

	Int2 operator* (int val)
	{
		Int2 result = Int2(this->x * val, this->y * val);
		return result;
	}

	Int2 operator/ (int val)
	{
		Int2 result = Int2(this->x / val, this->y / val);
		return result;
	}

	Int2 operator/ (float val)
	{
		Int2 result = Int2(this->x / val, this->y / val);
		return result;
	}

	operator Float2() const
	{
		return Float2(this->x, this->y);
	}
};

#endif