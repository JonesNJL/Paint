#ifndef BUTTON_GUI_ELEMENT_CLASS_H
#define BUTTON_GUI_ELEMENT_CLASS_H

#include "CroppedGuiElement.h"
#include "Controls.h"
#include "GuiEventManager.h"

class ButtonGuiElement : public CroppedGuiElement
{
	private:
		GuiEventManager* guiEventManager;
		std::string event;
	public:
		ButtonGuiElement() = default;
		ButtonGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, GuiEventManager* guiEventManager, std::string event);
		void UpdateVisuals();
		void UpdateSelected(Controls& controls);
};

#endif