#ifndef REGION_ALIGNMENT_CLASS_H
#define REGION_ALIGNMENT_CLASS_H

struct RegionAlignment
{
	int pos = 0;

	int min = 0;
	int max = 0;

	bool canMove = false;

	RegionAlignment() = default;

	RegionAlignment(int pos, bool canMove)
	{
		this->pos = pos;
		this->canMove = canMove;
	}
};

#endif