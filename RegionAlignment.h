#ifndef REGION_ALIGNMENT_CLASS_H
#define REGION_ALIGNMENT_CLASS_H

struct RegionAlignment
{
	int pos;

	int min;
	int max;

	bool canMove;

	RegionAlignment() = default;

	RegionAlignment(int pos, bool canMove)
	{
		this->pos = pos;
		this->canMove = canMove;
	}
};

#endif