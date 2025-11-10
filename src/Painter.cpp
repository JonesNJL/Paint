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

void Painter::Paint(unsigned int imageID, Int2 pixelPos)
{
	activeColor = Color::HSVtoRGB(hue, saturation, brightness);
	layer.SetPixel(pixelPos, activeColor);
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