#include "CroppedGuiElement.h"

CroppedGuiElement::CroppedGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID)
	: GuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID)
{
	minUV = Ratio2(Ratio(0, 1), Ratio(0, 1));
	maxUV = Ratio2(Ratio(1, 1), Ratio(1, 1));
}

void CroppedGuiElement::SetAlignments(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, int regionPadding)
{
	this->alignmentLeft = left;
	this->alignmentRight = right;
	this->alignmentTop = top;
	this->alignmentBottom = bottom;
	this->regionPadding = regionPadding;
}

void CroppedGuiElement::UpdateCropping()
{
	if (imageID == 0) { return; }

	Int2 regionScale = Int2(alignmentRight->pos - alignmentLeft->pos, alignmentTop->pos - alignmentBottom->pos);
	Float2 halfScale = scale / 2;
	Int2 scaleInt2 = Int2(scale.x, scale.y);

	Float2 anchorRelativeToBL = (*anchor) - Float2(alignmentLeft->pos, alignmentBottom->pos);
	Float2 localPositionBL = anchorRelativeToBL + localPosition;

	Int2 uncroppedLocalMin = Int2(localPositionBL.x - halfScale.x, (localPositionBL.y - halfScale.y));

	Int2 visualLocalMin = Int2(fmax(regionPadding, localPositionBL.x - halfScale.x), fmax(regionPadding, localPositionBL.y - halfScale.y));
	Int2 visualLocalMax = Int2(fmin(regionScale.x - regionPadding, localPositionBL.x + halfScale.x), fmin(regionScale.y - regionPadding, localPositionBL.y + halfScale.y));
	
	minUV = Ratio2(visualLocalMin - uncroppedLocalMin, scaleInt2);
	maxUV = Ratio2(visualLocalMax - uncroppedLocalMin, scaleInt2);

	if (imageID >= 65536)
	{
		imageRegistry->UpdateRendering(imageID, minUV, maxUV);
	}
}

void CroppedGuiElement::AddRenderData(std::vector<unsigned int>& verts, std::vector<GLuint>& indices)
{
	int vertCount = verts.size() / 8;

	Float2 screenPosition = (*anchor) + localPosition;
	Float2 halfScale = scale / 2;

	int minX = (alignmentLeft != nullptr) ? alignmentLeft->pos + regionPadding : -69420;
	int maxX = (alignmentRight != nullptr) ? alignmentRight->pos - regionPadding : 69420;
	int maxY = (alignmentTop != nullptr) ? alignmentTop->pos - regionPadding : 69420;
	int minY = (alignmentBottom != nullptr) ? alignmentBottom->pos + regionPadding : -69420;

	Int2 vertBottomLeft = Int2(fmax(minX, screenPosition.x - halfScale.x), fmax(minY, screenPosition.y - halfScale.y));
	Int2 vertBottomRight = Int2(fmin(maxX, screenPosition.x + halfScale.x), fmax(minY, screenPosition.y - halfScale.y));
	Int2 vertTopLeft = Int2(fmax(minX, screenPosition.x - halfScale.x), fmin(maxY, screenPosition.y + halfScale.y));
	Int2 vertTopRight = Int2(fmin(maxX, screenPosition.x + halfScale.x), fmin(maxY, screenPosition.y + halfScale.y));

	if (vertBottomLeft.x >= vertBottomRight.x || vertTopLeft.x >= vertTopRight.x || vertBottomLeft.y >= vertTopLeft.y || vertBottomRight.y >= vertTopRight.y)
	{
		return;
	}

	//Vert 1

	verts.push_back(vertBottomLeft.x);
	verts.push_back(vertBottomLeft.y);
	verts.push_back(layer);

	verts.push_back(minUV.x.numerator);
	verts.push_back(minUV.x.denominator);
	verts.push_back(minUV.y.numerator);
	verts.push_back(minUV.y.denominator);

	verts.push_back(imageID);

	//Vert 2

	verts.push_back(vertBottomRight.x);
	verts.push_back(vertBottomRight.y);
	verts.push_back(layer);

	verts.push_back(maxUV.x.numerator);
	verts.push_back(maxUV.x.denominator);
	verts.push_back(minUV.y.numerator);
	verts.push_back(minUV.y.denominator);

	verts.push_back(imageID);

	//Vert 3

	verts.push_back(vertTopLeft.x);
	verts.push_back(vertTopLeft.y);
	verts.push_back(layer);

	verts.push_back(minUV.x.numerator);
	verts.push_back(minUV.x.denominator);
	verts.push_back(maxUV.y.numerator);
	verts.push_back(maxUV.y.denominator);

	verts.push_back(imageID);

	//Vert 4

	verts.push_back(vertTopRight.x);
	verts.push_back(vertTopRight.y);
	verts.push_back(layer);

	verts.push_back(maxUV.x.numerator);
	verts.push_back(maxUV.x.denominator);
	verts.push_back(maxUV.y.numerator);
	verts.push_back(maxUV.y.denominator);

	verts.push_back(imageID);



	indices.push_back(vertCount + 0);
	indices.push_back(vertCount + 1);
	indices.push_back(vertCount + 2);
	indices.push_back(vertCount + 2);
	indices.push_back(vertCount + 1);
	indices.push_back(vertCount + 3);
	//indices.push_back(vertCount + 0);
	//indices.push_back(vertCount + 1);
	//indices.push_back(vertCount + 3);

	//std::cout << "G Position: " + std::to_string(globalPosition.x) + "," + std::to_string(globalPosition.y) << std::endl;
}

Float2 CroppedGuiElement::GetGlobalMin()
{
	Float2 halfScale = scale / 2;
	Float2 globalPosition = *anchor + localPosition;
	return Float2(fmax(alignmentLeft->pos + regionPadding, globalPosition.x - halfScale.x), fmax(alignmentBottom->pos + regionPadding, globalPosition.y - halfScale.y));
}

Float2 CroppedGuiElement::GetGlobalMax()
{
	Float2 halfScale = scale / 2;
	Float2 globalPosition = *anchor + localPosition;
	return Float2(fmin(alignmentRight->pos - regionPadding, globalPosition.x + halfScale.x), fmin(alignmentTop->pos - regionPadding, globalPosition.y + halfScale.y));
}