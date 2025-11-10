#include "ButtonGuiElement.hpp"

ButtonGuiElement::ButtonGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, GuiEventManager* guiEventManager, std::string event)
	: CroppedGuiElement(localPosition, scale, layer, anchor, imageRegistry, imageID)
{
	this->guiEventManager = guiEventManager;
	this->event = event;
}

void ButtonGuiElement::UpdateVisuals()
{
	//Select visual stuff idk
}

void ButtonGuiElement::UpdateSelected(Controls& controls)
{
	if (controls.KeyHit("left_mouse"))
	{
		std::cout << "Button Hit!" << std::endl;
		guiEventManager->CallEvent(event);
	}
}