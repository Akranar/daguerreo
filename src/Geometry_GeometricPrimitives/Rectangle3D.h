#ifndef _RECTANGLE_3D_H_
#define _RECTANGLE_3D_H_

#include "ExLib_Eigen.h"

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

#endif