#ifndef CIRCLE_SLIDER_GUI_ELEMENT_CLASS_H
#define CIRCLE_SLIDER_GUI_ELEMENT_CLASS_H

#include "CroppedGuiElement.hpp"
#include "Controls.hpp"
#include "GuiEventManager.hpp"

class CircleSliderGuiElement : public CroppedGuiElement
{
	private:
		GuiEventManager* guiEventManager;
		
		float* linkedVariableAngle;
		float minAngle;
		float maxAngle;
		
		float* linkedVariableRadius;
		float minRadius;
		float maxRadius;
		
		CroppedGuiElement* indicatorElement;
	public:
		CircleSliderGuiElement() = default;
		CircleSliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, GuiEventManager* guiEventManager, float* linkedVariableAngle, float minAngle, float maxAngle, float* linkedVariableRadius, float minRadius, float maxRadius);
		void UpdateVisuals();
		void UpdateSelected(Controls& controls);
		bool IsMouseInBounds(Controls& controls);
		void SetIndicatorElement(CroppedGuiElement* indicatorElement);
		void UpdateIndicatorPosition();
};

#endif