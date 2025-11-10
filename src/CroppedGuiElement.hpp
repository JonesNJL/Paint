#ifndef CROPPED_GUI_ELEMENT_CLASS_H
#define CROPPED_GUI_ELEMENT_CLASS_H

#include "GuiElement.hpp"
#include "Controls.hpp"

class CroppedGuiElement : public GuiElement
{
	protected:
		RegionAlignment* alignmentLeft;
		RegionAlignment* alignmentRight;
		RegionAlignment* alignmentTop;
		RegionAlignment* alignmentBottom;
		int regionPadding;

		Ratio2 minUV;
		Ratio2 maxUV;
	
	public:
		CroppedGuiElement() = default;
		CroppedGuiElement(Float2 localPosition, Float2 scale, int layer, Float2* anchor, ImageRegistry* imageRegistry, unsigned int imageID);
	
		void SetAlignments(RegionAlignment* left, RegionAlignment* right, RegionAlignment* top, RegionAlignment* bottom, int regionPadding);

		void Update(Controls& controls) {}
		virtual void UpdateCropping();
	
		void AddRenderData(std::vector<unsigned int>& verts, std::vector<GLuint>& indices);

		Float2 GetGlobalMin();
		Float2 GetGlobalMax();
};

#endif