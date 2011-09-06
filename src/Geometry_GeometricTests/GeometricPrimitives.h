#ifndef GEOMETRIC_PRIMITIVES_H_
#define GEOMETRIC_PRIMITIVES_H_

#include "ExLib_Eigen.h"

#include <vector>

class Cone;
class Cylinder;
class Line;
class LineSegment;
class Plane;
class Polygon;
class Polyhedron;
class Ray;
class Rectangle3D;
class Tetrahedron;
class Triangle;
class Quadrilateral;
	
/*
	*****************
		Cone
	*****************
*/

class Cone
{
	Eigen::Vector3f tip, tip_to_endcap;
	float radius;
public:
	Cone(const Eigen::Vector3f & tip, const Eigen::Vector3f & tip_to_endcap, float radius)
		: tip(tip), tip_to_endcap(tip_to_endcap), radius(radius) {};

	inline const Eigen::Vector3f & GetTip() const;
	inline const Eigen::Vector3f & GetAxis() const;
	inline float GetRadius() const;

};

inline const Eigen::Vector3f & Cone::GetTip() const
{
	return tip;
}
inline const Eigen::Vector3f & Cone::GetAxis() const
{
	return tip_to_endcap;
}
inline float Cone::GetRadius() const
{
	return radius;
}

/*
	*****************
		Cylinder
	*****************
*/

class Cylinder
{
	Eigen::Vector3f axis;
	float radius;
	float length;
public:
	Cylinder(const Eigen::Vector3f & axis, float radius, float length)
		: axis(axis.normalized()), radius(radius), length(length)
	{};

	Cylinder(const Eigen::Vector3f & axis, float radius)
	{
		this->length = axis.norm();
		this->radius = radius;
		this->axis = axis.normalized();
	};

	inline const Eigen::Vector3f & GetAxis() const;
	inline float GetRadius() const;
};

inline const Eigen::Vector3f & Cylinder::GetAxis() const
{
	return axis;
}
inline float Cylinder::GetRadius() const
{
	return radius;
}

/*
	*****************
		Line
	*****************
*/

class Line
{
private:
	Eigen::Vector3f point, direction;
public:
	Line(const Eigen::Vector3f & point, const Eigen::Vector3f & direction)
		: point(point), direction(direction) {};

	inline const Eigen::Vector3f & GetPoint() const;
	inline const Eigen::Vector3f & GetDirection() const;
	inline void GetPointAt(float t, Eigen::Vector3f & rtn_point) const;
};

inline const Eigen::Vector3f & Line::GetPoint() const
{
	return point;
}

inline const Eigen::Vector3f & Line::GetDirection() const
{
	return direction;
}

inline void Line::GetPointAt(float t, Eigen::Vector3f & rtn_point) const
{
	rtn_point = GetPoint() + t * GetDirection();
}

/*
	*****************
		LineSegment
	*****************
*/

class LineSegment
{
	Eigen::Vector3f point_a, point_b;
public:
	LineSegment(const Eigen::Vector3f & point_a, const Eigen::Vector3f & point_b)
		: point_a(point_a), point_b(point_b) {};

	inline const Eigen::Vector3f & GetPointA() const;
	inline const Eigen::Vector3f & GetPointB() const;
	inline void GetPointAt(float t, Eigen::Vector3f & rtn_point) const;
};

inline const Eigen::Vector3f & LineSegment::GetPointA() const
{
	return point_a;
}

inline const Eigen::Vector3f & LineSegment::GetPointB() const
{
	return point_b;
}

inline void LineSegment::GetPointAt(float t, Eigen::Vector3f & rtn_point) const
{
	rtn_point = GetPointA() + t * (GetPointB() - GetPointA());
}

/*
	*****************
		Plane
	*****************

	Plane Representations:
		-> Ax + By + Cz + D = 0
			-> Where X = <x, y, z>
			-> Where <A, B, C> = plane normal.
		-> As plane-vector:
			-> <A, B, C, D>
		-> dot(n, X-P) = 0
				-> where n = plane normal.
				-> where P = point on plane.
			-> dot(n, X-P) = dot(n, X) - dot(n, P) = 0
				-> dot(n, X) + D = 0
					-> where D = -dot(n, P)
*/

class Plane
{
	Eigen::Vector3f normal;
	float d;
public:
	Plane(const Eigen::Vector3f & normal, float d)
		: normal(normal.normalized()), d(d)
	{
	}
	Plane(const Eigen::Vector4f & plane_vector)
	{
		normal[0] = plane_vector[0];
		normal[1] = plane_vector[1];
		normal[2] = plane_vector[2];
		normal.normalize();
		d = plane_vector[3];
	}

	inline const Eigen::Vector3f & GetNormal() const;
	inline float GetD() const;

};

inline const Eigen::Vector3f & Plane::GetNormal() const
{
	return normal;
}
inline float Plane::GetD() const
{
	return d;
}

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

/*
	*****************
		Ray
	*****************
*/

class Ray
{
private:
	Eigen::Vector3f origin, direction;
public:
	Ray(const Eigen::Vector3f & origin, const Eigen::Vector3f & direction)
		: origin(origin), direction(direction) {};
	inline const Eigen::Vector3f & GetOrigin() const;
	inline const Eigen::Vector3f & GetDirection() const;
};

inline const Eigen::Vector3f & Ray::GetOrigin() const
{
	return origin;
}

inline const Eigen::Vector3f & Ray::GetDirection() const
{
	return direction;
}

/*
	*****************
		Rectangle3D
	*****************
*/

class Rectangle3D
{
	Eigen::Vector3f center, axis_a, axis_b;
	float half_extent_a, half_extent_b;
public:
	Rectangle3D(const Eigen::Vector3f & center, const Eigen::Vector3f & axis_a, const Eigen::Vector3f & axis_b)
	{
		this->center = center;
		this->axis_a = axis_a;
		this->axis_b = axis_b;
		this->half_extent_a = axis_a.norm();
		this->half_extent_b = axis_b.norm();
		this->axis_a.normalize();
		this->axis_b.normalize();
	}

	Rectangle3D(const Eigen::Vector3f & center, const Eigen::Vector3f & axis_a, const Eigen::Vector3f & axis_b, const float half_extent_a, const float half_extent_b)
		: center(center), axis_a(axis_a), axis_b(axis_b), half_extent_a(half_extent_a), half_extent_b(half_extent_b)
	{
	}

	inline const Eigen::Vector3f & GetCenter() const;
	inline const Eigen::Vector3f & GetAxisA() const;
	inline const Eigen::Vector3f & GetAxisB() const;
	inline float GetExtentA() const;
	inline float GetExtentB() const;
};

inline const Eigen::Vector3f & Rectangle3D::GetCenter() const
{
	return center;
}
inline const Eigen::Vector3f & Rectangle3D::GetAxisA() const
{
	return axis_a;
}
inline const Eigen::Vector3f & Rectangle3D::GetAxisB() const
{
	return axis_b;
}
inline float Rectangle3D::GetExtentA() const
{
	return half_extent_a;
}
inline float Rectangle3D::GetExtentB() const
{
	return half_extent_b;
}

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