#ifndef GUI_ELEMENT_CLASS_H
#define GUI_ELEMENT_CLASS_H

#include "Util.h"
#include "RegionAlignment.h"
#include "vector"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "ImageRegistry.h"
#include "Controls.h"

class GuiElement
{
	protected:
		Float2 localPosition;
		Float2 scale; // width / height
		int layer;
		Float2* anchor;

		ImageRegistry* imageRegistry;
		unsigned int imageID;

	public:
		GuiElement() = default;
		GuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID);

		void SetLocalPosition(Float2 localPosition);
		void SetScale(Float2 scale);

		Float2 GetLocalPosition();

		void SetImage(unsigned int imageID);

		virtual void AddRenderData(std::vector<unsigned int>& verts, std::vector<GLuint>& indices);

		virtual void UpdateVisuals() {}
		virtual void UpdateSelected(Controls& controls) {}

		virtual Float2 GetGlobalMin();
		virtual Float2 GetGlobalMax();
		virtual bool IsMouseInBounds(Controls& controls);

		//void Unselect();
};

#endif