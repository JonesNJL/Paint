#ifndef PAINTER_H
#define PAINTER_H

#include "ImageRegistry.h"
#include "Util.h"

class Painter
{
	private:
		Image layer;
		ImageRegistry* imageRegistry;
		int activeLayerID;
		const Int2 canvasSize = Int2(64, 64);
		Color activeColor;
		float brightness;
		float hue;
		float saturation;
	public:
		Painter() = default;
		Painter(ImageRegistry* imageRegistry);
		void Paint(unsigned int imageID, Int2 pixelPos);
		Image& GetLayer(); //Eventually will be replaced with a way to either:
												//get all layers visible given a render image id from the image registry
												//calculate the final visible image given a render image id from the image registry and the min/max pixels on the view
		Int2 GetCanvasSize();
		void SetActiveColor(Color color);
		float* GetVariable(std::string variableName);
};

#endif