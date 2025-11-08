#include "GuiEventManager.h"

GuiEventManager::GuiEventManager(Painter* painter)
{
	this->painter = painter;
}

void GuiEventManager::CallEvent(std::string name)
{
	if (name == "painter_setcolor_red") { Event_PainterSetColorRed(); }
	else if (name == "painter_setcolor_green") { Event_PainterSetColorGreen(); }
	else (std::cout << "Event Name \"" << name << "\" not found!" << std::endl);
}

void GuiEventManager::Event_PainterSetColorRed()
{
	painter->SetActiveColor(Color(255, 0, 0, 255));
}

void GuiEventManager::Event_PainterSetColorGreen()
{
	painter->SetActiveColor(Color(0, 255, 0, 255));
}