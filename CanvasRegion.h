#ifndef CANVAS_REGION_CLASS_H
#define CANVAS_REGION_CLASS_H

#include "Region.h"
#include "Painter.h"

class CanvasRegion : public Region
{
	private:
		unsigned int canvasImageID;
		CanvasGuiElement* canvasElement;

		Float2 canvasOrigin;
		float canvasZoom;


	public:
		CanvasRegion();
		CanvasRegion(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter);
		void Update(Controls& controls);
};

#endif