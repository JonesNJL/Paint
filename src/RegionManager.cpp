#include "RegionManager.hpp"

void RegionManager::Init(ImageRegistry* imageRegistry, GuiEventManager* guiEventManager, Painter* painter, Int2 screenSize)
{
	std::cout << "but it refused" << std::endl;

	this->imageRegistry = imageRegistry;
	this->guiEventManager = guiEventManager;
	this->painter = painter;

	RegionAlignment* initLeftAlignment = new RegionAlignment(0, false);
	RegionAlignment* initRightAlignment = new RegionAlignment(screenSize.x, false);
	RegionAlignment* initTopAlignment = new RegionAlignment(screenSize.y, false);
	RegionAlignment* initBottomAlignment = new RegionAlignment(0, false);

	Region* initRegion = new CanvasRegion(initLeftAlignment, initRightAlignment, initTopAlignment, initBottomAlignment, imageRegistry, guiEventManager, painter);
	regions.push_back(initRegion);

	regionAlignments.emplace(initLeftAlignment, List<Region*> { initRegion });
	regionAlignments.emplace(initRightAlignment, List<Region*> { initRegion });
	regionAlignments.emplace(initTopAlignment, List<Region*> { initRegion });
	regionAlignments.emplace(initBottomAlignment, List<Region*> { initRegion });
}

RegionManager::~RegionManager()
{
	for (int i = 0; i < regions.size(); i++)
	{
		delete regions[i];
	}

	std::map<RegionAlignment*, List<Region*>>::iterator it;
	for (it = regionAlignments.begin(); it != regionAlignments.end(); it++)
	{
		delete it->first;
	}
}

void RegionManager::GetRenderData(std::vector<unsigned int>& verts, std::vector<GLuint>& indices)
{
	for (int i = 0; i < regions.size(); i++)
	{
		std::vector<GuiElement*> currentElements = regions[i]->GetGuiElements();
		
		for (int j = 0; j < currentElements.size(); j++)
		{
			currentElements[j]->AddRenderData(verts, indices);
		}
	}
}

void RegionManager::UpdateRegions(Controls& controls)
{
	if (!controls.KeyDown("left_mouse"))
	{
		if (controls.KeyRelease("left_mouse")) { UpdateAlignmentBounds(); }

		resizeAlignment = nullptr;
		currentlySelectedGuiElement = nullptr;
	}

	if (resizeAlignment != nullptr)
	{
		//Handle Resizing

		int activeMouseAxis = 0;
		if (resizeVertical) { activeMouseAxis = controls.MousePos().y; }
		else { activeMouseAxis = controls.MousePos().x; }

		if (activeMouseAxis < resizeAlignment->min)
		{
			resizeAlignment->pos = resizeAlignment->min;
		}
		else if (activeMouseAxis > resizeAlignment->max)
		{
			resizeAlignment->pos = resizeAlignment->max;
		}
		else
		{
			resizeAlignment->pos = activeMouseAxis;
		}

		List<Region*>& alignedRegions = regionAlignments[resizeAlignment];
		for (int i = 0; i < alignedRegions.Size(); i++)
		{
			if (alignedRegions[i] == nullptr)
			{
				std::cout << "wwww" << std::endl;
			}

			alignedRegions[i]->Resize(resizeAlignment);
		}
	}
	else if (currentlySelectedGuiElement != nullptr)
	{
		//Handing Gui Element Interaction
		currentlySelectedGuiElement->UpdateSelected(controls);
	}
	else
	{
		//Get Active Region

		Region* activeRegion = nullptr;
		for (int i = 0; i < regions.size(); i++)
		{
			if (regions[i]->CheckBounds(controls.MousePos()))
			{
				activeRegion = regions[i];
				break;
			}
		}

		if (activeRegion != nullptr)
		{
			//Check for new resizing

			if (controls.KeyHit("left_mouse"))
			{
				int borderSide = 0;
				(*activeRegion).CheckBorders(controls.MousePos(), resizeAlignment, resizeVertical);
				if (resizeAlignment != nullptr && !resizeAlignment->canMove) { resizeAlignment = nullptr; }
			}

			if (controls.KeyHit("left_bracket")) //Split, temporary, TODO: make better split
			{
				SplitRegion(activeRegion, false);
			}
			else if (controls.KeyHit("right_bracket"))
			{
				SplitRegion(activeRegion, true);
			}

			//Check for new selected element

			for (int i = 0; i < regions.size(); i++)
			{
				activeRegion->GetSelectedElement(controls, currentlySelectedGuiElement);
			}

			if (currentlySelectedGuiElement != nullptr)
			{
				currentlySelectedGuiElement->UpdateSelected(controls);
			}

			//Update region (non-element interactions, such as scaling a canvas)

			if (resizeAlignment == nullptr && currentlySelectedGuiElement == nullptr)
			{
				activeRegion->Update(controls);
			}
		}
	}

	//Update region and GuiElement visuals

	for (int i = 0; i < regions.size(); i++)
	{
		regions[i]->UpdateVisuals();
	}
}

void RegionManager::SplitRegion(Region* region, bool vertical)
{
	Region* newRegion;
	RegionAlignment* newAlignment = new RegionAlignment();

	List<RegionAlignment*> alignments = region->GetAlignments();
	RegionAlignment* alignmentLeft = alignments[0];
	RegionAlignment* alignmentRight = alignments[1];
	RegionAlignment* alignmentTop = alignments[2];
	RegionAlignment* alignmentBottom = alignments[3];

	if (vertical)
	{
		//Create split line
		int splitLine = (alignmentBottom->pos + alignmentTop->pos) / 2;
		newAlignment->pos = splitLine;
		newAlignment->canMove = true;
		newAlignment->min = alignmentBottom->pos + region->minSize;
		newAlignment->max = alignmentTop->pos - region->minSize;

		//Create new region and set alignments for it
		newRegion = new MenuRegion(alignmentLeft, alignmentRight, alignmentTop, newAlignment, imageRegistry, guiEventManager, painter);

		//Set alignments for old region
		region->SetAlignments(alignmentLeft, alignmentRight, newAlignment, alignmentBottom);
		region->Resize(newAlignment);

		//Update alignments' aligned regions
		regionAlignments.emplace(newAlignment, List<Region*> {region, newRegion});
		regionAlignments[alignmentLeft].Add(newRegion);
		regionAlignments[alignmentRight].Add(newRegion);
		regionAlignments[alignmentTop].Add(newRegion);
		regionAlignments[alignmentTop].Remove(region);
	}
	else
	{
		//Create split line
		int splitLine = (alignmentLeft->pos + alignmentRight->pos) / 2;
		newAlignment->pos = splitLine;
		newAlignment->canMove = true;
		newAlignment->min = alignmentLeft->pos + region->minSize;
		newAlignment->max = alignmentRight->pos - region->minSize;

		//Create new region and set alignments for it
		newRegion = new MenuRegion(newAlignment, alignmentRight, alignmentTop, alignmentBottom, imageRegistry, guiEventManager, painter);

		//Set alignments for old region
		region->SetAlignments(alignmentLeft, newAlignment, alignmentTop, alignmentBottom);
		region->Resize(newAlignment);

		//Update alignments' aligned regions
		regionAlignments.emplace(newAlignment, List<Region*> {region, newRegion});
		regionAlignments[alignmentTop].Add(newRegion);
		regionAlignments[alignmentBottom].Add(newRegion);
		regionAlignments[alignmentRight].Add(newRegion);
		regionAlignments[alignmentRight].Remove(region);
	}

	regions.push_back(newRegion);
	UpdateAlignmentBounds();
}

void RegionManager::UpdateAlignmentBounds()
{
	std::map<RegionAlignment*, List<Region*>>::iterator it;
	for (it = regionAlignments.begin(); it != regionAlignments.end(); it++)
	{
		RegionAlignment* alignment = it->first;
		if (!alignment->canMove) { continue; } //Alignment is locked, should never move (for the borders)
		List<Region*>& alignedRegions = it->second;

		int currentMax = 999999; //Really big number
		int currentMin = -999999; //Really small number

		for (int i = 0; i < alignedRegions.Size(); i++)
		{
			Region* region = alignedRegions[i];
			RegionAlignment* oppositeAlignment = region->GetOppositeAlignment(alignment);

			if (oppositeAlignment->pos < alignment->pos && oppositeAlignment->pos + region->minSize > currentMin)
			{
				currentMin = oppositeAlignment->pos + region->minSize;
			}
			else if (oppositeAlignment->pos > alignment->pos && oppositeAlignment->pos - region->minSize < currentMax)
			{
				currentMax = oppositeAlignment->pos - region->minSize;
			}
		}

		alignment->min = currentMin;
		alignment->max = currentMax;
	}
}