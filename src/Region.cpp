#include "Region.hpp"

Region::Region() {}

Region::Region(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter)
{
	SetAlignments(left, right, top, bottom);

	this->imageRegistry = imageRegistry;
	this->guiEventManager = guiEventManager;
	this->painter = painter;

	// v only child regions will create their own, but will probably still be created in child constructors

	int regionWidth = right->pos - left->pos;
	int regionHeight = top->pos - bottom->pos;

	float halfPadding = padding / 2.0;
	int doublePadding = padding * 2;
	backgroundTopLeft = CreateGuiElement(Float2(halfPadding, -halfPadding), Float2(padding, padding), 0, &anchorTopLeft, imageRegistry, 7);
	backgroundTopRight = CreateGuiElement(Float2(-halfPadding, -halfPadding), Float2(padding, padding), 0, &anchorTopRight, imageRegistry, 9);
	backgroundBottomLeft = CreateGuiElement(Float2(halfPadding, halfPadding), Float2(padding, padding), 0, &anchorBottomLeft, imageRegistry, 1);
	backgroundBottomRight = CreateGuiElement(Float2(-halfPadding, halfPadding), Float2(padding, padding), 0, &anchorBottomRight, imageRegistry, 3);
	backgroundLeft = CreateGuiElement(Float2(halfPadding, 0), Float2(padding, regionHeight - doublePadding), 0, &anchorLeft, imageRegistry, 4);
	backgroundRight = CreateGuiElement(Float2(-halfPadding, 0), Float2(padding, regionHeight - doublePadding), 0, &anchorRight, imageRegistry, 6);
	backgroundTop = CreateGuiElement(Float2(0, -halfPadding), Float2(regionWidth - doublePadding, padding), 0, &anchorTop, imageRegistry, 8);
	backgroundBottom = CreateGuiElement(Float2(0, halfPadding), Float2(regionWidth - doublePadding, padding), 0, &anchorBottom, imageRegistry, 2);
	backgroundCenter = CreateGuiElement(Float2(0, 0), Float2(regionWidth - doublePadding, regionHeight - doublePadding), 0, &anchorCenter, imageRegistry, 5);
}

Region::~Region()
{
	for (int i = 0; i < guiElements.size(); i++)
	{
		delete guiElements[i];
	}
}

void Region::SetAlignments(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom)
{
	this->alignmentLeft = left;
	this->alignmentRight = right;
	this->alignmentTop = top;
	this->alignmentBottom = bottom;

	UpdateAnchors();
	SetCroppedGuiElementAlignments();
}

void Region::SetCroppedGuiElementAlignments()
{
	for (int j = 0; j < croppedGuiElements.size(); j++)
	{
		croppedGuiElements[j]->SetAlignments(alignmentLeft, alignmentRight, alignmentTop, alignmentBottom, padding);
	}
}

List<RegionAlignment*> Region::GetAlignments()
{
	return List<RegionAlignment*> {alignmentLeft, alignmentRight, alignmentTop, alignmentBottom};
}

std::vector<GuiElement*> Region::GetGuiElements()
{
	return guiElements;
}

bool Region::CheckBounds(Int2 mousePos)
{
	if (mousePos.x > alignmentLeft->pos && mousePos.x < alignmentRight->pos && mousePos.y > alignmentBottom->pos && mousePos.y < alignmentTop->pos)
	{
		return true;
	}

	return false;
}

void Region::CheckBorders(Int2 mousePos, RegionAlignment*& borderAlignment, bool& borderVertical)
{
	int minRegionWidth = 30;
	int borderThickness = 7;

	if (isMouseWithinBounds(alignmentLeft->pos, alignmentBottom->pos, alignmentLeft->pos + borderThickness, alignmentTop->pos, mousePos)) //Left border
	{
		std::cout << "Left" << std::endl;
		borderAlignment = alignmentLeft;
		borderVertical = false;
	}
	else if (isMouseWithinBounds(alignmentRight->pos - borderThickness, alignmentBottom->pos, alignmentRight->pos, alignmentTop->pos, mousePos)) //Right border
	{
		std::cout << "Right" << std::endl;
		borderAlignment = alignmentRight;
		borderVertical = false;
	}
	else if (isMouseWithinBounds(alignmentLeft->pos, alignmentBottom->pos, alignmentRight->pos, alignmentBottom->pos + borderThickness, mousePos)) //Bottom Border
	{
		std::cout << "Bottom" << std::endl;
		borderAlignment = alignmentBottom;
		borderVertical = true;
	}
	else if (isMouseWithinBounds(alignmentLeft->pos, alignmentTop->pos - borderThickness, alignmentRight->pos, alignmentTop->pos, mousePos)) //Top Border
	{
		std::cout << "Top" << std::endl;
		borderAlignment = alignmentTop;
		borderVertical = true;
	}
}

bool Region::isMouseWithinBounds(int minX, int minY, int maxX, int maxY, Int2 mousePos)
{
	if (mousePos.x > minX && mousePos.x < maxX && mousePos.y > minY && mousePos.y < maxY)
	{
		return true;
	}

	return false;
}

void Region::Resize(RegionAlignment* resizeAlignment)
{
	UpdateAnchors();

	int regionWidth = alignmentRight->pos - alignmentLeft->pos;
	int regionHeight = alignmentTop->pos - alignmentBottom->pos;

	int doublePadding = padding * 2;
	backgroundCenter->SetScale(Float2(regionWidth - doublePadding, regionHeight - doublePadding));
	backgroundLeft->SetScale(Float2(padding, regionHeight - doublePadding));
	backgroundRight->SetScale(Float2(padding, regionHeight - doublePadding));
	backgroundTop->SetScale(Float2(regionWidth - doublePadding, padding));
	backgroundBottom->SetScale(Float2(regionWidth - doublePadding, padding));

	UpdateCroppedElements();
}

void Region::UpdateAnchors()
{
	anchorTopLeft = Float2(alignmentLeft->pos, alignmentTop->pos);
	anchorTopRight = Float2(alignmentRight->pos, alignmentTop->pos);
	anchorBottomLeft = Float2(alignmentLeft->pos, alignmentBottom->pos);
	anchorBottomRight = Float2(alignmentRight->pos, alignmentBottom->pos);
	anchorLeft = Float2(alignmentLeft->pos, (alignmentTop->pos + alignmentBottom->pos) / 2.0);
	anchorRight = Float2(alignmentRight->pos, (alignmentTop->pos + alignmentBottom->pos) / 2.0);
	anchorTop = Float2((alignmentLeft->pos + alignmentRight->pos) / 2.0, alignmentTop->pos);
	anchorBottom = Float2((alignmentLeft->pos + alignmentRight->pos) / 2.0, alignmentBottom->pos);
	anchorCenter = Float2((alignmentLeft->pos + alignmentRight->pos) / 2.0, (alignmentTop->pos + alignmentBottom->pos) / 2.0);
}

RegionAlignment* Region::GetOppositeAlignment(RegionAlignment* alignment)
{
	if (alignment == alignmentLeft)
	{
		return alignmentRight;
	}
	else if (alignment == alignmentRight)
	{
		return alignmentLeft;
	}
	else if (alignment == alignmentTop)
	{
		return alignmentBottom;
	}
	else if (alignment == alignmentBottom)
	{
		return alignmentTop;
	}
	else
	{
		std::cout << "Region not aligned to given alignment!" << std::endl;
		return nullptr;
	}
}

void Region::UpdateVisuals()
{
	for (int i = 0; i < guiElements.size(); i++)
	{
		guiElements[i]->UpdateVisuals();
	}
}

void Region::GetSelectedElement(Controls& controls, GuiElement*& selectedElement)
{
	for (int i = 0; i < guiElements.size(); i++)
	{
		if (guiElements[i]->IsMouseInBounds(controls) && controls.KeyHit("left_mouse"))
		{
			selectedElement = guiElements[i];
		}
	}
}

void Region::UpdateCroppedElements()
{
	for (int i = 0; i < croppedGuiElements.size(); i++)
	{
		croppedGuiElements[i]->UpdateCropping();
	}
}

GuiElement* Region::CreateGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID)
{
	GuiElement* newElement = new GuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID);
	guiElements.push_back(newElement);

	return newElement;
}

CroppedGuiElement* Region::CreateCroppedGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID)
{
	CroppedGuiElement* newElement = new CroppedGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID);
	newElement->SetAlignments(alignmentLeft, alignmentRight, alignmentTop, alignmentBottom, padding);
	guiElements.push_back(newElement);
	croppedGuiElements.push_back(newElement);

	return newElement;
}

ButtonGuiElement* Region::CreateButtonGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, std::string event)
{
	ButtonGuiElement* newElement = new ButtonGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID, guiEventManager, event);
	newElement->SetAlignments(alignmentLeft, alignmentRight, alignmentTop, alignmentBottom, padding);
	guiElements.push_back(newElement);
	croppedGuiElements.push_back(newElement);
	buttonGuiElements.push_back(newElement);

	return newElement;
}

SliderGuiElement* Region::CreateSliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, float* linkedVariable, float min, float max)
{
	SliderGuiElement* newElement = new SliderGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID, guiEventManager, linkedVariable, min, max);
	newElement->SetAlignments(alignmentLeft, alignmentRight, alignmentTop, alignmentBottom, padding);
	guiElements.push_back(newElement);
	croppedGuiElements.push_back(newElement);
	sliderGuiElements.push_back(newElement);

	CroppedGuiElement* indicatorElement = CreateCroppedGuiElement(localPosition, Float2(5, scale.y), layer, anchor, imageRegistry, 12);
	newElement->SetIndicatorElement(indicatorElement);

	return newElement;
}

CircleSliderGuiElement* Region::CreateCircleSliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, float* linkedVariableAngle, float minAngle, float maxAngle, float* linkedVariableRadius, float minRadius, float maxRadius)
{
	CircleSliderGuiElement* newElement = new CircleSliderGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID, guiEventManager, linkedVariableAngle, minAngle, maxAngle, linkedVariableRadius, minRadius, maxRadius);
	newElement->SetAlignments(alignmentLeft, alignmentRight, alignmentTop, alignmentBottom, padding);
	guiElements.push_back(newElement);
	croppedGuiElements.push_back(newElement);
	circleSliderGuiElements.push_back(newElement);

	CroppedGuiElement* indicatorElement = CreateCroppedGuiElement(localPosition, Float2(10, 10), layer, anchor, imageRegistry, 10);
	newElement->SetIndicatorElement(indicatorElement);

	return newElement;
}

CanvasGuiElement* Region::CreateCanvasGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID)
{
	CanvasGuiElement* newElement = new CanvasGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID, painter);
	newElement->SetAlignments(alignmentLeft, alignmentRight, alignmentTop, alignmentBottom, padding);
	guiElements.push_back(newElement);
	croppedGuiElements.push_back(newElement);
	canvasGuiElements.push_back(newElement);

	return newElement;
}