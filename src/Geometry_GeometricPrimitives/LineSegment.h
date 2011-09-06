#ifndef _LINE_SEGMENT_H_
#define _LINE_SEGMENT_H_

#include "ExLib_Eigen.h"

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

#endif