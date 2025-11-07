#ifndef REGIONMANAGER_H
#define REGIONMANAGER_H

#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "Util.h"
#include "Region.h"
#include "CanvasRegion.h"
#include "MenuRegion.h"
#include "ImageRegistry.h"
#include "Painter.h"
#include "Controls.h"

class RegionManager
{
	private:
		std::vector<Region*> regions;
		std::map<RegionAlignment*, List<Region*>> regionAlignments;

		RegionAlignment* resizeAlignment = nullptr;
		bool resizeVertical = false;

		GuiElement* currentlySelectedGuiElement = nullptr;

		ImageRegistry* imageRegistry;
		GuiEventManager* guiEventManager;
		Painter* painter;

		bool test = false;

	public:
		void Init(ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter);
		void GetRenderData(std::vector<unsigned int>& verts, std::vector<GLuint>& indices);
		void UpdateRegions(Controls& controls);
		void SplitRegion(Region* region, bool vertical);
		void UpdateAlignmentBounds();
};

#endif