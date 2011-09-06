#ifndef _TETRAHEDRON_H_
#define _TETRAHEDRON_H_

#include "ExLib_Eigen.h"

/*
	*****************
		Tetrahedron
	*****************
*/

class Tetrahedron
{
	Eigen::Vector3f vertices [4];
public:
	Tetrahedron(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c, const Eigen::Vector3f & d)
	{
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;
		vertices[3] = d;
	}
	Tetrahedron(const Eigen::Vector3f arg_vertices[4])
	{
		vertices[0] = arg_vertices[0];
		vertices[1] = arg_vertices[1];
		vertices[2] = arg_vertices[2];
	}

	inline const Eigen::Vector3f & GetVertex(int index) const;
	inline const Eigen::Vector3f * GetVertices() const;
};

inline const Eigen::Vector3f & Tetrahedron::GetVertex(int index) const
{
	return vertices[index % 4];
}

inline const Eigen::Vector3f * Tetrahedron::GetVertices() const
{
	return vertices;
}

#endif