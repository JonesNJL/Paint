#ifndef MENU_REGION_CLASS_H
#define MENU_REGION_CLASS_H

#include "Region.h"
#include "Painter.h"

class MenuRegion : public Region
{
	private:
		Painter* painter;
		
		ButtonGuiElement* redButtonElement;
		ButtonGuiElement* greenButtonElement;
		SliderGuiElement* brightnessElement;
		CircleSliderGuiElement* hueSatElement;
		SliderGuiElement* hueElement;
		SliderGuiElement* saturationElement;
	
	public:
		MenuRegion();
		MenuRegion(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter);
};

#endif