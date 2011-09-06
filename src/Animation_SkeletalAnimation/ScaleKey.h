#ifndef _SCALE_KEY_H_
#define _SCALE_KEY_H_

#include "ExLib_Eigen.h"

class ScaleKey
{
protected:
	double time;
	Eigen::Vector3f scale;
public:
	ScaleKey() {};
	ScaleKey(double time, const Eigen::Vector3f & scale)
		:
		time(time), 
		scale(scale)
	{
	};
	inline double GetTime() const;
	inline const Eigen::Vector3f & GetScale() const;
	inline void SetScale(const float * in);

	inline void SetTime(double in);
	inline void SetScale(Eigen::Vector3f & in);
};

inline double ScaleKey::GetTime() const
{
	return time;
}

inline const Eigen::Vector3f & ScaleKey::GetScale() const
{
	return scale;
}

inline void ScaleKey::SetTime(double in)
{
	time = in;
}

inline void ScaleKey::SetScale(Eigen::Vector3f & in)
{
	scale = in;
}

inline void ScaleKey::SetScale(const float * in)
{
	scale[0] = in[0];
	scale[1] = in[1];
	scale[2] = in[2];
}

#endif