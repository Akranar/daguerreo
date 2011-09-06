#ifndef _POLYHEDRON_H_
#define _POLYHEDRON_H_

#include "Plane.h"
#include <vector>

/*
	*****************
		Polyhedron
	*****************
*/

class Polyhedron
{
	std::vector<Plane> planes;
public:
	Polyhedron() {};
	virtual ~Polyhedron() {};
	inline void AddPlane(const Plane & plane);
	inline const Plane & GetPlane(int index) const;
	inline int GetPlaneCount() const;
};

inline void Polyhedron::AddPlane(const Plane & plane)
{
	planes.push_back(plane);
}

inline const Plane & Polyhedron::GetPlane(int index) const
{
	return planes[index];
}

inline int Polyhedron::GetPlaneCount() const
{
	return planes.size();
}

#endif