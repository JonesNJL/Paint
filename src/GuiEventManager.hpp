#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <map>
#include "Util.hpp"
#include "Painter.hpp"

class GuiEventManager
{
	private:
		Painter* painter;
	public:
		GuiEventManager() = default;
		GuiEventManager(Painter* painter);
		void CallEvent(std::string name);

		void Event_PainterSetColorRed();
		void Event_PainterSetColorGreen();
};

#endif