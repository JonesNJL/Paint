#include "Image.hpp"

Image::Image(Int2 size, Color color)
{
	this->size = size;

	for (int y = 0; y < size.y; y++)
	{
		data.Add(List<Color>());

		for (int x = 0; x < size.x; x++)
		{
			data[y].Add(color);
		}
	}
}

Int2 Image::GetSize()
{
	return size;
}

List<int> Image::GetInts()
{
	List<int> newList(size.x * size.y);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			
			newList.Add(data[y][x].GetInt());
		}
	}

	return newList;
}

void Image::SetPixel(Int2 pixelPos, Color color)
{
	if (pixelPos.x >= 0 && pixelPos.y >= 0 && pixelPos.x < size.x && pixelPos.y < size.y)
	{
		data[pixelPos.y][pixelPos.x] = color;
	}
}

Color Image::GetPixel(Int2 pixelPos)
{
	if (pixelPos.x >= 0 && pixelPos.y >= 0 && pixelPos.x < size.x && pixelPos.y < size.y)
	{
		return data[pixelPos.y][pixelPos.x];
	}

	return Color();
}

Image Image::SubImage(Int2 min, Int2 max)
{
	Image subImage(max - min + Int2(1, 1));

	for (int y = min.y; y <= max.y; y++)
	{
		for (int x = min.x; x <= max.x; x++)
		{
			subImage.SetPixel(Int2(x - min.x, y - min.y), GetPixel(Int2(x, y)));
		}
	}

	return subImage;
}

void Image::GenerateCheckerboard(Color color1, Color color2)
{
	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			if ((y + x) % 2 == 0)
			{
				data[y][x] = color1;
			}
			else
			{
				data[y][x] = color2;
			}
		}
	}
}