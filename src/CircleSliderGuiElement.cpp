#include "CircleSliderGuiElement.hpp"

CircleSliderGuiElement::CircleSliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, GuiEventManager* guiEventManager, float* linkedVariableAngle, float minAngle, float maxAngle, float* linkedVariableRadius, float minRadius, float maxRadius)
	: CroppedGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID)
{
	this->guiEventManager = guiEventManager;
	
	this->linkedVariableAngle = linkedVariableAngle;
	this->minAngle = minAngle;
	this->maxAngle = maxAngle;

	this->linkedVariableRadius = linkedVariableRadius;
	this->minRadius = minRadius;
	this->maxRadius = maxRadius;
}

void CircleSliderGuiElement::UpdateVisuals()
{
	UpdateIndicatorPosition();
}

void CircleSliderGuiElement::UpdateSelected(Controls& controls)
{
	Float2 halfScale = scale / 2;
	float xFactor = (controls.MousePos().x - ((*anchor).x + localPosition.x)) / halfScale.x;
	float yFactor = (controls.MousePos().y - ((*anchor).y + localPosition.y)) / halfScale.y;

	float radius = Min(1, sqrt((xFactor * xFactor) + (yFactor * yFactor)));
	*linkedVariableRadius = minRadius + ((maxRadius - minRadius) * radius);

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
	*linkedVariableAngle = minAngle + ((maxAngle - minAngle) * angle);

	UpdateIndicatorPosition();
}

bool CircleSliderGuiElement::IsMouseInBounds(Controls& controls)
{
	Float2 globalPosition = (*anchor) + localPosition;
	float distanceX = globalPosition.x - controls.MousePos().x;
	float distanceY = globalPosition.y - controls.MousePos().y;
	return (sqrt((distanceX * distanceX) + (distanceY * distanceY)) <= scale.x / 2);
}

void CircleSliderGuiElement::SetIndicatorElement(CroppedGuiElement* indicatorElement)
{
	this->indicatorElement = indicatorElement;
	UpdateIndicatorPosition();
}

void CircleSliderGuiElement::UpdateIndicatorPosition()
{
	Float2 halfScale = scale / 2;
	Float2 globalPosition = *anchor + localPosition;
	Float2 globalMin = Float2(fmax(alignmentLeft->pos + regionPadding, globalPosition.x - halfScale.x), fmax(alignmentBottom->pos + regionPadding, globalPosition.y - halfScale.y));

	float displayRadius = halfScale.x * ((*linkedVariableRadius - minRadius) / (maxRadius - minRadius));
	float displayAngle = 6.28318530718 * ((*linkedVariableAngle - minAngle) / (maxAngle - minAngle));
	Float2 displayAngleVector = Float2(cos(displayAngle), sin(displayAngle));

	indicatorElement->SetLocalPosition(Float2(localPosition.x + (displayAngleVector.x * displayRadius), localPosition.y + (displayAngleVector.y * displayRadius)));
}