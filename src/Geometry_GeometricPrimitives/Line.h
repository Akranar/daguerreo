#ifndef _LINE_H_
#define _LINE_H_

#include "ExLib_Eigen.h"

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

#endif