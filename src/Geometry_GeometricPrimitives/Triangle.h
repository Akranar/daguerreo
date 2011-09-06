#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

/*
	*****************
		Triangle
	*****************
*/

class Triangle
{
	Eigen::Vector3f cclockwise_vertices [3];
public:
	Triangle(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c)
	{
		cclockwise_vertices[0] = a;
		cclockwise_vertices[1] = b;
		cclockwise_vertices[2] = c;
	}
	Triangle(const Eigen::Vector3f cc_vertices[3])
	{
		cclockwise_vertices[0] = cc_vertices[0];
		cclockwise_vertices[1] = cc_vertices[1];
		cclockwise_vertices[2] = cc_vertices[2];
	}

	inline const Eigen::Vector3f & GetVertex(int index) const;
	inline const Eigen::Vector3f * GetVertices() const;
};

inline const Eigen::Vector3f & Triangle::GetVertex(int index) const
{
	return cclockwise_vertices[index % 3];
}

inline const Eigen::Vector3f * Triangle::GetVertices() const
{
	return cclockwise_vertices;
}

#endif