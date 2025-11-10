#include "ImageRegistry.hpp"

ImageRegistry::ImageRegistry()
{
	//Register all of the GUI sprites, probably

	LoadGuiSprites();
}

void ImageRegistry::SetLayerReference(Image* layer)
{
	this->layer = layer;
}

void ImageRegistry::UpdateRendering(int id, Ratio2 minUV, Ratio2 maxUV)
{
	//TODO: Set the rendering's min and size to just be the nessasary, visible pixels
	//TODO: Potentially pass back the UVs representing the old UVs within the new bounds

	renderings[id].min = Int2(0, 0);
	renderings[id].size = Int2(layer->GetSize());
}

Image& ImageRegistry::GetImage(unsigned int id)
{
	if (id <= 65535)
	{
		if (sprites.count(id) == 0)
		{
			std::cout << "Image with ID of " << id << " not found in sprites!" << std::endl;
			Image image;
			return image;
		}

		return sprites[id];
	}
	
	std::cout << "Cannot get image of ID " << id << " as it is a rendering!" << std::endl;
	Image image;
	return image;
}

unsigned int ImageRegistry::GetOpenRenderingID()
{
	for (unsigned int i = 65536; i < 70000; i++)
	{
		if (renderings.find(i) == renderings.end())
		{
			return i;
		}
	}

	std::cout << "No open ID found!" << std::endl;
	return 0;
}

unsigned int* ImageRegistry::GetData(int& length)
{
	List<unsigned int> dataList;

	unsigned int numberOfImages = 0;

	std::map<unsigned int, Image>::iterator itS;
	std::map<unsigned int, CanvasRendering>::iterator itR;
	for (itS = sprites.begin(); itS != sprites.end(); itS++) { numberOfImages += 1; }
	for (itR = renderings.begin(); itR != renderings.end(); itR++) { numberOfImages += 1; }

	dataList.Add(numberOfImages);
	unsigned int currentStartingIndex = 4 * numberOfImages;
	
	for (itS = sprites.begin(); itS != sprites.end(); itS++)
	{
		unsigned int id = itS->first;
		Image& sprite = itS->second;

		dataList.Add(id); //ID
		dataList.Add(currentStartingIndex); //Starting index
		dataList.Add(sprite.GetSize().x); //Width
		dataList.Add(sprite.GetSize().y); //Height

		currentStartingIndex += sprite.GetSize().x * sprite.GetSize().y;
	}

	for (itR = renderings.begin(); itR != renderings.end(); itR++)
	{
		unsigned int id = itR->first;
		CanvasRendering& rendering = itR->second;

		dataList.Add(id); //ID
		dataList.Add(currentStartingIndex); //Starting index
		dataList.Add(rendering.size.x); //Width
		dataList.Add(rendering.size.y); //Height

		currentStartingIndex += rendering.size.x * rendering.size.y;
	}

	GetSpriteInts(dataList);
	GetRenderingInts(dataList);
	
	length = dataList.Size();
	unsigned int* data = new unsigned int[length];

	for (int i = 0; i < length; i++)
	{
		data[i] = dataList[i];
	}

	return data;
}

void ImageRegistry::GetSpriteInts(List<unsigned int>& dataList)
{
	std::map<unsigned int, Image>::iterator it;
	for (it = sprites.begin(); it != sprites.end(); it++)
	{
		Image& image = it->second;

		List<int> imageInts = image.GetInts();

		for (int i = 0; i < imageInts.Size(); i++)
		{
			dataList.Add(imageInts[i]);
		}
	}
}

void ImageRegistry::GetRenderingInts(List<unsigned int>& dataList)
{
	std::map<unsigned int, CanvasRendering>::iterator it;
	for (it = renderings.begin(); it != renderings.end(); it++)
	{
		List<int> imageInts = layer->GetInts(); //Will eventually calculated based on the rendering's visible layers

		for (int i = 0; i < imageInts.Size(); i++)
		{
			dataList.Add(imageInts[i]);
		}
	}
}

void ImageRegistry::LoadGuiSprites()
{
	Image nullSprite = LoadPNG("resources/null.png");
	sprites[0] = nullSprite;

	Image regionBackgroundSprites = LoadPNG("resources/gui/region_background.png");
	sprites[1] = regionBackgroundSprites.SubImage(Int2(0, 0), Int2(7, 7)); //Bottom Left
	sprites[2] = regionBackgroundSprites.SubImage(Int2(8, 0), Int2(8, 7)); //Bottom
	sprites[3] = regionBackgroundSprites.SubImage(Int2(9, 0), Int2(16, 7)); //Bottom Right
	sprites[4] = regionBackgroundSprites.SubImage(Int2(0, 8), Int2(6, 8)); //Left
	sprites[5] = regionBackgroundSprites.SubImage(Int2(8, 8), Int2(8, 8)); //Middle
	sprites[6] = regionBackgroundSprites.SubImage(Int2(9, 8), Int2(16, 8)); //Right
	sprites[7] = regionBackgroundSprites.SubImage(Int2(0, 9), Int2(6, 16)); //Top Left
	sprites[8] = regionBackgroundSprites.SubImage(Int2(8, 9), Int2(8, 16)); //Top
	sprites[9] = regionBackgroundSprites.SubImage(Int2(9, 9), Int2(16, 16)); //Top Right

	Image buttonSprite = LoadPNG("resources/gui/button.png");
	sprites[10] = buttonSprite;

	Image sliderSprite = LoadPNG("resources/gui/slider.png");
	sprites[11] = sliderSprite;

	Image sliderIndicatorSprite = LoadPNG("resources/gui/slider_indicator.png");
	sprites[12] = sliderIndicatorSprite;

	Image brightnessSliderSprite = GenerateBrightnessSliderSprite();
	sprites[13] = brightnessSliderSprite;

	Image hueSatSliderSprite = GenerateHueSatSliderSprite();
	sprites[14] = hueSatSliderSprite;
}

Image ImageRegistry::GenerateBrightnessSliderSprite()
{
	Image newImage = Image(Int2(256, 1));

	for (int i = 0; i <= 255; i++)
	{
		float factor = i / (float)255;
		Color color = Color(factor * 255, factor * 255, factor * 255, 255);
		newImage.SetPixel(Int2(i, 0), color);
	}

	return newImage;
}

Image ImageRegistry::GenerateHueSatSliderSprite()
{
	Image newImage = Image(Int2(511, 511));

	for (int y = -255; y <= 255; y++)
	{
		for (int x = -255; x <= 255; x++)
		{
			float xFactor = x / (float)255;
			float yFactor = y / (float)255;

			float angle = atan((yFactor / xFactor)) / 6.28318530718;
			if (xFactor < 0)
			{
				angle += 0.5;
			}
			else if (xFactor == 0 && yFactor >= 0)
			{
				angle = 0.25;
			}
			else if (xFactor == 0 && yFactor < 0)
			{
				angle = 0.75;
			}
			angle = Mod(angle, 1);
			float hue = angle;

			float saturation = sqrt((xFactor * xFactor) + (yFactor * yFactor));
			float brightness = 1;

			Color color = Color::HSVtoRGB(hue, saturation, brightness);
			if (saturation > 1)
			{
				color = Color(0, 0, 0, 0);
			}

			newImage.SetPixel(Int2(x + 255, y + 255), color);
		}
	}

	return newImage;
}