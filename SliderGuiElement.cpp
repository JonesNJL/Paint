#include "SliderGuiElement.h"

SliderGuiElement::SliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, GuiEventManager* guiEventManager, float* linkedVariable, float min, float max)
	: CroppedGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID)
{
	this->guiEventManager = guiEventManager;
	this->linkedVariable = linkedVariable;
	this->min = min;
	this->max = max;
}

void SliderGuiElement::UpdateVisuals()
{
	UpdateIndicatorPosition();
}

void SliderGuiElement::UpdateSelected(Controls& controls)
{
	float xFactor = (controls.MousePos().x - GetGlobalMin().x) / scale.x;
	float xValue = min + ((max - min) * xFactor);

	if (xValue < min)
	{
		xValue = min;
	}
	else if (xValue > max)
	{
		xValue = max;
	}

	*linkedVariable = xValue;
	UpdateIndicatorPosition();
}

void SliderGuiElement::SetIndicatorElement(CroppedGuiElement* indicatorElement)
{
	this->indicatorElement = indicatorElement;
	UpdateIndicatorPosition();
}

void SliderGuiElement::UpdateIndicatorPosition()
{
	Float2 halfScale = scale / 2;
	Float2 globalPosition = *anchor + localPosition;
	Float2 globalMin = Float2(fmax(alignmentLeft->pos + regionPadding, globalPosition.x - halfScale.x), fmax(alignmentBottom->pos + regionPadding, globalPosition.y - halfScale.y));

	float xPos = (localPosition.x - halfScale.x) + (scale.x * ((*linkedVariable - min) / (max - min)));
	indicatorElement->SetLocalPosition(Float2(xPos, indicatorElement->GetLocalPosition().y));
}