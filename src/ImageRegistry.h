#ifndef IMAGE_REGISTRY_CLASS_H
#define IMAGE_REGISTRY_CLASS_H

#include "Image.h"
#include <map>
#include <iostream>
#include "PNG.h"

class ImageRegistry
{
	// ID's 0-65535 are reserved for precreated sprites (0 being none)
	// ID's 65536 and up are for canvas renderings

	private:

		struct CanvasRendering
		{
			int layerID; //Will be list of layers eventually
			Int2 min;
			Int2 size;
		};

		std::map<unsigned int, Image> sprites;
		std::map<unsigned int, CanvasRendering> renderings;

		Image* layer;

	public:
		ImageRegistry();
		void SetLayerReference(Image* layer); //Called from Painter during its construction
		void UpdateRendering(int id, Ratio2 minUV, Ratio2 maxUV);
		Image& GetImage(unsigned int id);
		unsigned int GetOpenRenderingID();
		unsigned int* GetData(int& length);
		void GetSpriteInts(List<unsigned int>& dataList);
		void GetRenderingInts(List<unsigned int>& dataList);
		void LoadGuiSprites();
		Image GenerateBrightnessSliderSprite();
		Image GenerateHueSatSliderSprite();
};

#endif