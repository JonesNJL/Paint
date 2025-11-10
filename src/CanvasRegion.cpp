#include "CanvasRegion.hpp"

CanvasRegion::CanvasRegion() : Region() {}

CanvasRegion::CanvasRegion(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter)
	: Region(left, right, top, bottom, imageRegistry, guiEventManager, painter)
{
	canvasZoom = 400;
	canvasOrigin = Float2((left->pos + right->pos - canvasZoom) / 2, (top->pos + bottom->pos - canvasZoom) / 2);

	canvasElement = CreateCanvasGuiElement(canvasOrigin + Float2(canvasZoom / 2, canvasZoom / 2), Float2(canvasZoom, canvasZoom), 1, &anchorBottomLeft, imageRegistry, 0);

	canvasImageID = imageRegistry->GetOpenRenderingID();
	//imageRegistry->Depict(0, canvasImageID);
	//imageRegistry->CalculateImage(canvasImageID, painter->GetLayer()); //Normally will be set to the visible section of the layer

	canvasElement->SetImage(canvasImageID);
	canvasElement->UpdateCropping();
}

void CanvasRegion::Update(Controls& controls)
{
	Float2 localMouse = Float2(controls.MousePos().x, controls.MousePos().y) - anchorBottomLeft;
	Float2 focusedPos = (localMouse - canvasOrigin) / canvasZoom; // Floats from 0 to 1 (like a UV) on where the mouse is on the image

	if (controls.KeyDown("q")) { canvasZoom -= 0.1; }
	if (controls.KeyDown("e")) { canvasZoom += 0.1; }

	canvasOrigin = localMouse - (focusedPos * canvasZoom);

	float panAmount = canvasZoom * 0.001;
	if (controls.KeyDown("a")) { canvasOrigin.x -= panAmount; }
	if (controls.KeyDown("d")) { canvasOrigin.x += panAmount; }
	if (controls.KeyDown("w")) { canvasOrigin.y += panAmount; }
	if (controls.KeyDown("s")) { canvasOrigin.y -= panAmount; }

	Float2 canvasElementLocalPos = canvasOrigin + Float2(canvasZoom / 2.0, canvasZoom / 2.0);
	canvasElement->SetLocalPosition(canvasElementLocalPos);
	canvasElement->SetScale(Float2(canvasZoom, canvasZoom));

	canvasElement->UpdateCropping();
}