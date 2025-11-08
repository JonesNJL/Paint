#ifndef CANVAS_GUI_ELEMENT_CLASS_H
#define CANVAS_GUI_ELEMENT_CLASS_H

#include "CroppedGuiElement.h"
#include "Controls.h"
#include "GuiEventManager.h"

class CanvasGuiElement : public CroppedGuiElement
{
	private:
		Painter* painter;
	public:
		CanvasGuiElement() = default;
		CanvasGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, Painter* painter);
		void UpdateVisuals();
		void UpdateSelected(Controls& controls);
};

#endif