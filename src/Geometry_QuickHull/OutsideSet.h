#ifndef _OUTSIDE_SET_H_
#define _OUTSIDE_SET_H_

#include <set>
#include <vector>
#include <limits.h>
#include "ExLib_Topology.h"

class OutsideSet
{
private:
	std::vector<unsigned int> data;
	WingedTriangle * associated_triangle;
	float max_distance;
	unsigned int farthest_point;
public:
	OutsideSet(WingedTriangle * triangle) : associated_triangle(triangle), max_distance(0), farthest_point(INT_MAX) {};
	inline void Insert(unsigned int index, float distance)
	{
		if (distance > max_distance)
		{
			max_distance = distance;
			farthest_point = index;
		}
		return data.push_back(index);
	}
	inline unsigned int GetFarthest() const
	{
		return farthest_point;
	}

	inline unsigned int operator[] (int index)
	{
		return data[index];
	}
	inline void Clear()
	{
		data.clear();
	}
	inline unsigned int Size() const
	{
		return data.size();
	}
	inline WingedTriangle * GetTriangle() const
	{
		return associated_triangle;
	}
};

#endif