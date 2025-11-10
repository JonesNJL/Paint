#ifndef FLOAT2_H
#define FLOAT2_H

struct Float2
{
	float x = 0;
	float y = 0;

	Float2() = default;

	Float2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Float2 operator+ (const Float2& other)
	{
		Float2 result = Float2(this->x + other.x, this->y + other.y);
		return result;
	}

	Float2 operator- (const Float2& other)
	{
		Float2 result = Float2(this->x - other.x, this->y - other.y);
		return result;
	}

	Float2 operator* (float val)
	{
		Float2 result = Float2(this->x * val, this->y * val);
		return result;
	}

	Float2 operator/ (float val)
	{
		Float2 result = Float2(this->x / val, this->y / val);
		return result;
	}

	Float2 operator/ (Float2 val)
	{
		Float2 result = Float2(this->x / val.x, this->y / val.y);
		return result;
	}
};

#endif