#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "ExLib_Eigen.h"
#include <vector>

/*
	*****************
		Polygon
	*****************
*/

class Polygon
{
	std::vector<Eigen::Vector3f> cc_vertices;
public:
	Polygon() {};
	virtual ~Polygon() {};
	inline void AddPlane(const Eigen::Vector3f & vertex);
	inline const Eigen::Vector3f & GetVertex(int index) const;
	inline int GetVertexCount() const;
};

inline void Polygon::AddPlane(const Eigen::Vector3f & vertex)
{
	cc_vertices.push_back(vertex);
}

inline const Eigen::Vector3f & Polygon::GetVertex(int index) const
{
	return cc_vertices[index];
}

inline int Polygon::GetVertexCount() const
{
	return cc_vertices.size();
}

#endif