#include "CanvasGuiElement.hpp"

CanvasGuiElement::CanvasGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, Painter* painter)
	: CroppedGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID)
{
	this->painter = painter;
}

void CanvasGuiElement::UpdateVisuals()
{
	
}

void CanvasGuiElement::UpdateSelected(Controls& controls)
{
	Float2 mouseFloat2 = Float2(controls.MousePos().x, controls.MousePos().y);
	Float2 uncroppedGlobalMin = (*anchor) + localPosition - (scale / 2);
	Float2 focusedPos = (mouseFloat2 - uncroppedGlobalMin) / scale.x; // Floats from 0 to 1 (like a UV) on where the mouse is on the image

	Int2 canvasSize = painter->GetCanvasSize();
	Int2 pixelPos = Int2(floor(focusedPos.x * canvasSize.x), floor(focusedPos.y * canvasSize.y));
	painter->Paint(imageID, pixelPos, controls.KeyHit("left_mouse"));
}