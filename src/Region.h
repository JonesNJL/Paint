#ifndef REGION_CLASS_H
#define REGION_CLASS_H

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include "RegionAlignment.h"
#include "GuiElement.h"
#include "CroppedGuiElement.h"
#include "ButtonGuiElement.h"
#include "SliderGuiElement.h"
#include "CircleSliderGuiElement.h"
#include "CanvasGuiElement.h"
#include "Controls.h"

class Region
{
	public:
		RegionAlignment* alignmentLeft = nullptr;
		RegionAlignment* alignmentRight = nullptr;
		RegionAlignment* alignmentTop = nullptr;
		RegionAlignment* alignmentBottom = nullptr;

		Float2 anchorTopLeft;
		Float2 anchorTopRight;
		Float2 anchorBottomLeft;
		Float2 anchorBottomRight;
		Float2 anchorLeft;
		Float2 anchorRight;
		Float2 anchorTop;
		Float2 anchorBottom;
		Float2 anchorCenter;

		std::vector<GuiElement*> guiElements;
		std::vector<CroppedGuiElement*> croppedGuiElements;
		std::vector<ButtonGuiElement*> buttonGuiElements;
		std::vector<SliderGuiElement*> sliderGuiElements;
		std::vector<CircleSliderGuiElement*> circleSliderGuiElements;
		std::vector<CanvasGuiElement*> canvasGuiElements;

		GuiElement* backgroundTopLeft = nullptr;
		GuiElement* backgroundTopRight = nullptr;
		GuiElement* backgroundBottomLeft = nullptr;
		GuiElement* backgroundBottomRight = nullptr;
		GuiElement* backgroundLeft = nullptr;
		GuiElement* backgroundRight = nullptr;
		GuiElement* backgroundTop = nullptr;
		GuiElement* backgroundBottom = nullptr;
		GuiElement* backgroundCenter = nullptr;

		const int minSize = 30;
		const int padding = 8;

		ImageRegistry* imageRegistry = nullptr;
		GuiEventManager* guiEventManager = nullptr;
		Painter* painter = nullptr;

		Region();
		Region(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter);
		void SetAlignments(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom);
		void SetCroppedGuiElementAlignments();
		List<RegionAlignment*> GetAlignments();
		std::vector<GuiElement*> GetGuiElements();
		bool CheckBounds(Int2 mousePos);
		void CheckBorders(Int2 mousePos, RegionAlignment*& borderAlignment, bool& borderVertical);
		bool isMouseWithinBounds(int minX, int minY, int maxX, int maxY, Int2 mousePos);
		void Resize(RegionAlignment* resizeAlignment);
		void UpdateAnchors();
		RegionAlignment* GetOppositeAlignment(RegionAlignment* alignment);
		virtual void Update(Controls& controls) {}
		void UpdateVisuals();
		void GetSelectedElement(Controls& controls, GuiElement*& selectedElement);
		void UpdateCroppedElements();

		GuiElement* CreateGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID);
		CroppedGuiElement* CreateCroppedGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID);
		ButtonGuiElement* CreateButtonGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, std::string event);
		SliderGuiElement* CreateSliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, float* linkedVariable, float min, float max);
		CircleSliderGuiElement* CreateCircleSliderGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID, float* linkedVariableAngle, float minAngle, float maxAngle, float* linkedVariableRadius, float minRadius, float maxRadius);
		CanvasGuiElement* CreateCanvasGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID);
};

#endif