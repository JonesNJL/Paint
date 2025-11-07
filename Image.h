#ifndef IMAGE_H
#define IMAGE_H

#include "Math.h"
#include "Color.h"

class Image
{
	private:
		List<List<Color>> data;
		Int2 size = Int2(0, 0);

	public:
		Image() = default;
		Image(Int2 size) : Image(size, Color(255, 255, 255, 255)) {} //Default is opaque white
		Image(Int2 size, Color color);
		Int2 GetSize();
		List<int> GetInts();
		void SetPixel(Int2 pixelPos, Color color);
		Color GetPixel(Int2 pixelPos);
		Image SubImage(Int2 min, Int2 max);
		void GenerateCheckerboard(Color color1, Color color2);
};

#endif