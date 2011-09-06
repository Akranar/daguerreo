#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "ExLib_Eigen.h"

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

#endif