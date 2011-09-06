#ifndef _ROTATION_KEY_H_
#define _ROTATION_KEY_H_

#include "ExLib_Eigen.h"
#include "ExLib_Serialization.h"

class RotationKey//: public Serializable
{
	float rotation[4];
	double time;
public:
	RotationKey() {};
	RotationKey(double time, const Eigen::Quaternionf & rot)
		:
		time(time)
	{
		rotation[0] = rot.x();
		rotation[1] = rot.y();
		rotation[2] = rot.z();
		rotation[3] = rot.w();
	};
	inline double GetTime() const;
	inline const float * GetRotation() const;

	inline void SetTime(double in);
	inline void SetRotation(Eigen::Quaternionf & in);
	inline void SetRotation(const float * in);
	//Serialization
	/*
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
	*/
};

inline double RotationKey::GetTime() const
{
	return time;
}

inline const float * RotationKey::GetRotation() const
{
	return rotation;
}

inline void RotationKey::SetTime(double in)
{
	time = in;
}

inline void RotationKey::SetRotation(Eigen::Quaternionf & in)
{
	rotation[0] = in.x();
	rotation[1] = in.y();
	rotation[2] = in.z();
	rotation[3] = in.w();
}

inline void RotationKey::SetRotation(const float * in)
{
	rotation[0] = in[0];
	rotation[1] = in[1];
	rotation[2] = in[2];
	rotation[3] = in[3];
}

#endif