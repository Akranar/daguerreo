#ifndef _CONE_H_
#define _CONE_H_

#include "ExLib_Eigen.h"

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

#endif