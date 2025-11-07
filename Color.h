#ifndef COLOR_H
#define COLOR_H

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	struct Swizzle2
	{
		unsigned char x;
		unsigned char y;
		Swizzle2(unsigned char x, unsigned char y) { this->x = x; this->y = y; }
	};

	struct Swizzle3
	{
		unsigned char x;
		unsigned char y;
		unsigned char z;
		Swizzle3(unsigned char x, unsigned char y, unsigned char z) { this->x = x; this->y = y; this->z = z; }
	};

	Color() = default;

	Color(int red, int green, int blue) : Color(red, green, blue, 255) {}

	Color(int red, int green, int blue, int alpha)
	{
		this->r = red;
		this->g = green;
		this->b = blue;
		this->a = alpha;
	}

	Color(Swizzle2 redgreen, int blue, int alpha) : Color(redgreen.x, redgreen.y, blue, alpha) {}
	Color(int red, Swizzle2 greenblue, int alpha) : Color(red, greenblue.x, greenblue.y, alpha) {}
	Color(int red, int green, Swizzle2 bluealpha) : Color(red, green, bluealpha.x, bluealpha.y) {}

	Color(Swizzle3 redgreenblue, int alpha) : Color(redgreenblue.x, redgreenblue.y, redgreenblue.z, alpha) {}
	Color(int red, Swizzle3 greenbluealpha) : Color(red, greenbluealpha.x, greenbluealpha.y, greenbluealpha.z) {}

	Swizzle2 rg() { return Swizzle2(r, g); }
	Swizzle2 rb() { return Swizzle2(r, b); }
	Swizzle2 ra() { return Swizzle2(r, a); }
	Swizzle2 gb() { return Swizzle2(g, b); }
	Swizzle2 ga() { return Swizzle2(g, a); }
	Swizzle2 ba() { return Swizzle2(b, a); }

	Swizzle3 rgb() { return Swizzle3(r, g, b); }
	Swizzle3 rga() { return Swizzle3(r, g, a); }
	Swizzle3 rba() { return Swizzle3(r, b, a); }
	Swizzle3 gba() { return Swizzle3(g, b, a); }

	int GetInt()
	{
		return (16777216 * r) + (65536 * g) + (256 * b) + a;
	}

	Color operator+ (const Color& other)
	{
		Color result = Color(this->r + other.r, this->g + other.g, this->b + other.b, this->a + other.a);
		return result;
	}

	static Color HSVtoRGB(float hue, float saturation, float value)
	{
		float c = value * saturation;
		float x = c * (1 - abs(Mod((hue * 6), 2) - 1));
		float m = value - c;

		float rPrime, gPrime, bPrime;

		if (((0 / 6.0) <= hue && hue < (1 / 6.0)) || hue == 1)
		{
			rPrime = c; gPrime = x, bPrime = 0;
		}
		else if ((1 / 6.0) <= hue && hue < (2 / 6.0))
		{
			rPrime = x; gPrime = c, bPrime = 0;
		}
		else if ((2 / 6.0) <= hue && hue < (3 / 6.0))
		{
			rPrime = 0; gPrime = c, bPrime = x;
		}
		else if ((3 / 6.0) <= hue && hue < (4 / 6.0))
		{
			rPrime = 0; gPrime = x, bPrime = c;
		}
		else if ((4 / 6.0) <= hue && hue < (5 / 6.0))
		{
			rPrime = x; gPrime = 0, bPrime = c;
		}
		else if ((5 / 6.0) <= hue && hue < (6 / 6.0))
		{
			rPrime = c; gPrime = 0, bPrime = x;
		}

		return Color((rPrime + m) * 255, (gPrime + m) * 255, (bPrime + m) * 255, 255);
	}
};

#endif