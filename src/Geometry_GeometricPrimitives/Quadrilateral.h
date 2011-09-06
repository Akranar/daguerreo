#ifndef _QUADRILATERAL_H_
#define _QUADRILATERAL_H_

#include "ExLib_Eigen.h"

/*
	*****************
		Quadrilateral
	*****************
*/

class Quadrilateral
{
	Eigen::Vector3f cclockwise_vertices [4];
public:
	Quadrilateral(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c, const Eigen::Vector3f & d)
	{
		cclockwise_vertices[0] = a;
		cclockwise_vertices[1] = b;
		cclockwise_vertices[2] = c;
		cclockwise_vertices[3] = d;
	}
	Quadrilateral(const Eigen::Vector3f cc_vertices[4])
	{
		cclockwise_vertices[0] = cc_vertices[0];
		cclockwise_vertices[1] = cc_vertices[1];
		cclockwise_vertices[2] = cc_vertices[2];
		cclockwise_vertices[3] = cc_vertices[3];
	}

	inline const Eigen::Vector3f & GetVertex(int index) const;
	inline const Eigen::Vector3f * GetVertices() const;
};

inline const Eigen::Vector3f & Quadrilateral::GetVertex(int index) const
{
	return cclockwise_vertices[index % 4];
}

inline const Eigen::Vector3f * Quadrilateral::GetVertices() const
{
	return cclockwise_vertices;
}

#endif