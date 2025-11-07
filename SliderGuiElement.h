#ifndef SLIDER_GUI_ELEMENT_CLASS_H
#define SLIDER_GUI_ELEMENT_CLASS_H

#include "CroppedGuiElement.h"
#include "Controls.h"
#include "GuiEventManager.h"

class SliderGuiElement : public CroppedGuiElement
{
	private:
		GuiEventManager* guiEventManager;
		float* linkedVariable;
		float min;
		float max;
		CroppedGuiElement* indicatorElement;
	public:
		SliderGuiElement() = default;
		SliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, GuiEventManager* guiEventManager, float* linkedVariable, float min, float max);
		void UpdateVisuals();
		void UpdateSelected(Controls& controls);
		void SetIndicatorElement(CroppedGuiElement* indicatorElement);
		void UpdateIndicatorPosition();
};

#endif