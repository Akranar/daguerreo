#ifndef _RAY_H_
#define _RAY_H_

#include "ExLib_Eigen.h"

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

#endif