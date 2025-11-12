#ifndef REGIONMANAGER_H
#define REGIONMANAGER_H

#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "Util.hpp"
#include "Region.hpp"
#include "CanvasRegion.hpp"
#include "MenuRegion.hpp"
#include "ImageRegistry.hpp"
#include "Painter.hpp"
#include "Controls.hpp"

class RegionManager
{
	private:
		std::vector<Region*> regions;
		std::map<RegionAlignment*, List<Region*>> regionAlignments;

		RegionAlignment* resizeAlignment = nullptr;
		bool resizeVertical = false;

		GuiElement* currentlySelectedGuiElement = nullptr;

		ImageRegistry* imageRegistry = nullptr;
		GuiEventManager* guiEventManager = nullptr;
		Painter* painter = nullptr;

		bool test = false;

	public:
		RegionManager() = default;
		void Init(ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter, Int2 screenSize);
		~RegionManager();
		void GetRenderData(std::vector<unsigned int>& verts, std::vector<GLuint>& indices);
		void UpdateRegions(Controls& controls);
		void SplitRegion(Region* region, bool vertical);
		void UpdateAlignmentBounds();
};

#endif