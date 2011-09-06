#ifndef _PLANE_H_
#define _PLANE_H_

#include "ExLib_Eigen.h"

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
	Plane()
		: normal(0,0,0), d(0)
	{}
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
	Plane(const float * plane_vector)
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

#endif