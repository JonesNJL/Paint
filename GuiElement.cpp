#include "GuiElement.h"

GuiElement::GuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID)
{
	this->localPosition = localPosition;
	this->scale = scale;
	this->layer = layer;
	this->anchor = anchor;
	this->imageRegistry = imageRegistry;
	this->imageID = imageID;
}

void GuiElement::SetLocalPosition(Float2 localPosition)
{
	this->localPosition = localPosition;
}

void GuiElement::SetScale(Float2 scale)
{
	this->scale = scale;
}

Float2 GuiElement::GetLocalPosition()
{
	return localPosition;
}

void GuiElement::SetImage(unsigned int imageID)
{
	this->imageID = imageID;
}

void GuiElement::AddRenderData(std::vector<unsigned int>& verts, std::vector<GLuint>& indices)
{
	int vertCount = verts.size() / 8;

	Float2 screenPosition = (*anchor) + localPosition;
	Float2 halfScale = scale / 2;

	Int2 vertBottomLeft = Int2(screenPosition.x - halfScale.x, screenPosition.y - halfScale.y);
	Int2 vertBottomRight = Int2(screenPosition.x + halfScale.x, screenPosition.y - halfScale.y);
	Int2 vertTopLeft = Int2(screenPosition.x - halfScale.x, screenPosition.y + halfScale.y);
	Int2 vertTopRight = Int2(screenPosition.x + halfScale.x, screenPosition.y + halfScale.y);

	if (vertBottomLeft.x >= vertBottomRight.x || vertTopLeft.x >= vertTopRight.x || vertBottomLeft.y >= vertTopLeft.y || vertBottomRight.y >= vertTopRight.y)
	{
		return;
	}

	//Vert 1

	verts.push_back(vertBottomLeft.x);
	verts.push_back(vertBottomLeft.y);
	verts.push_back(layer);

	verts.push_back(0);
	verts.push_back(1);
	verts.push_back(0);
	verts.push_back(1);

	verts.push_back(imageID);

	//Vert 2

	verts.push_back(vertBottomRight.x);
	verts.push_back(vertBottomRight.y);
	verts.push_back(layer);

	verts.push_back(1);
	verts.push_back(1);
	verts.push_back(0);
	verts.push_back(1);

	verts.push_back(imageID);

	//Vert 3

	verts.push_back(vertTopLeft.x);
	verts.push_back(vertTopLeft.y);
	verts.push_back(layer);

	verts.push_back(0);
	verts.push_back(1);
	verts.push_back(1);
	verts.push_back(1);

	verts.push_back(imageID);

	//Vert 4

	verts.push_back(vertTopRight.x);
	verts.push_back(vertTopRight.y);
	verts.push_back(layer);

	verts.push_back(1);
	verts.push_back(1);
	verts.push_back(1);
	verts.push_back(1);

	verts.push_back(imageID);



	indices.push_back(vertCount + 0);
	indices.push_back(vertCount + 1);
	indices.push_back(vertCount + 2);
	indices.push_back(vertCount + 2);
	indices.push_back(vertCount + 1);
	indices.push_back(vertCount + 3);
}

Float2 GuiElement::GetGlobalMin()
{
	Float2 halfScale = scale / 2;
	Float2 globalPosition = *anchor + localPosition;
	return globalPosition - halfScale;
}

Float2 GuiElement::GetGlobalMax()
{
	Float2 halfScale = scale / 2;
	Float2 globalPosition = *anchor + localPosition;
	return globalPosition + halfScale;
}

bool GuiElement::IsMouseInBounds(Controls& controls)
{
	return (controls.MousePos().x >= GetGlobalMin().x && controls.MousePos().x <= GetGlobalMax().x && controls.MousePos().y >= GetGlobalMin().y && controls.MousePos().y <= GetGlobalMax().y);
}