#include "MenuRegion.hpp"

MenuRegion::MenuRegion() {}

MenuRegion::MenuRegion(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter)
	: Region(left, right, top, bottom, imageRegistry, guiEventManager, painter)
{
	redButtonElement = CreateButtonGuiElement(Float2(25, -25), Float2(20, 20), 1, &anchorTopLeft, imageRegistry, 10, "painter_setcolor_red");
	greenButtonElement = CreateButtonGuiElement(Float2(55, -25), Float2(20, 20), 1, &anchorTopLeft, imageRegistry, 10, "painter_setcolor_green");
	brightnessElement = CreateSliderGuiElement(Float2(115, -25), Float2(80, 20), 1, &anchorTopLeft, imageRegistry, 13, painter->GetVariable("brightness"), 0, 1);
	hueSatElement = CreateCircleSliderGuiElement(Float2(205, -55), Float2(80, 80), 1, &anchorTopLeft, imageRegistry, 14, painter->GetVariable("hue"), 0, 1, painter->GetVariable("saturation"), 0, 1);
	hueElement = CreateSliderGuiElement(Float2(115, -55), Float2(80, 20), 1, &anchorTopLeft, imageRegistry, 11, painter->GetVariable("hue"), 0, 1);
	saturationElement = CreateSliderGuiElement(Float2(115, -85), Float2(80, 20), 1, &anchorTopLeft, imageRegistry, 11, painter->GetVariable("saturation"), 0, 1);
}