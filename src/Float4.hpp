#ifndef FLOAT4_H
#define FLOAT4_H

struct Float4
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	Float4() = default;

	Float4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Float4 operator+ (const Float4& other)
	{
		Float4 result = Float4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
		return result;
	}

	Float4 operator- (const Float4& other)
	{
		Float4 result = Float4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
		return result;
	}

	Float4 operator* (float val)
	{
		Float4 result = Float4(this->x * val, this->y * val, this->z * val, this->w * val);
		return result;
	}

	Float4 operator/ (float val)
	{
		Float4 result = Float4(this->x / val, this->y / val, this->z / val, this->w / val);
		return result;
	}
};

#endif