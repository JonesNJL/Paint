#include "Painter.hpp"

Painter::Painter(ImageRegistry* imageRegistry)
{
	this->imageRegistry = imageRegistry;

	//layer = Image(canvasSize, Color(255, 255, 255, 255));
	layer = LoadPNG("resources/Suzanne Small.png");

	imageRegistry->SetLayerReference(&layer);

	activeLayerID = 0;
	brightness = 0.2f;
	hue = 0;
	saturation = 0;

	activeColor = Color::HSVtoRGB(hue, saturation, brightness);
}

void Painter::Paint(unsigned int imageID, Int2 pixelPos, bool brushDown)
{
	activeColor = Color::HSVtoRGB(hue, saturation, brightness);

	if (brushDown)
	{
		layer.SetPixel(pixelPos, activeColor);
	}
	else
	{
		DrawLine(previousPaintedPos, pixelPos);
	}

	previousPaintedPos = pixelPos;
}

void Painter::DrawLine(Int2 startPos, Int2 endPos)
{
	Int2 delta = endPos - startPos;

	if (delta.x == 0 && delta.y == 0)
	{
		return;
	}

	if (abs(delta.y) > abs(delta.x)) // Y is longer
	{
		float slope = delta.x / (float)delta.y;

		for (int i = 0; i <= abs(delta.y); i++)
		{
			int j = i * Normalize(delta.y);

			Int2 pixelPos = startPos + Int2(j * slope, j);
			layer.SetPixel(pixelPos, activeColor);
		}
	}
	else // X is longer (or equal)
	{
		float slope = delta.y / (float)delta.x;

		for (int i = 0; i <= abs(delta.x); i++)
		{
			int j = i * Normalize(delta.x);

			Int2 pixelPos = startPos + Int2(j, j * slope);
			layer.SetPixel(pixelPos, activeColor);
		}
	}
}

Image& Painter::GetLayer()
{
	return layer;
}

Int2 Painter::GetCanvasSize()
{
	return canvasSize;
}

void Painter::SetActiveColor(Color color)
{
	activeColor = color;
}

float* Painter::GetVariable(std::string variableName)
{
	if (variableName == "brightness")
	{
		return &brightness;
	}
	else if (variableName == "hue")
	{
		return &hue;
	}
	else if (variableName == "saturation")
	{
		return &saturation;
	}
}