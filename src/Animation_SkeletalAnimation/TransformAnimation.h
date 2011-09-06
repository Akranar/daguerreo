/*
#ifndef _TRANSFORM_ANIMATION_H_
#define _TRANSFORM_ANIMATION_H_

//#include "TransformKey.h"
#include "ScaleKey.h"
#include "RotationKey.h"
#include "TranslationKey.h"

#include "ExLib_Serialization.h"
#include <vector>


class TransformAnimation: public Serializable
{
public:
	enum Behavior
	{
		DEFAULT,
		CONSTANT,
		LINEAR,
		REPEAT
	};
protected:
	double duration;
	double ticks_per_second;
	Behavior pre_behavior;
	Behavior post_behavior;
	//std::vector<TransformKey, Eigen::aligned_allocator<TransformKey> > keys;
	std::vector<ScaleKey> scale_keys;
	std::vector<RotationKey, Eigen::aligned_allocator<RotationKey> > rotation_keys;
	std::vector<TranslationKey> translation_keys;
	
public:
	TransformAnimation();
	virtual ~TransformAnimation();
	inline double GetDuration() const;
	inline double GetTicksPerSecond() const;
	inline Behavior GetPreBehavior() const;
	inline Behavior GetPostBehavior() const;

	inline void SetDuration(double in_duration);
	inline void SetTicksPerSecond(double in_ticks_per_second);
	inline void SetPreBehavior(Behavior in_pre_behavior);
	inline void SetPostBehavior(Behavior in_post_behavior);

	void GetInterpolatedTransform(double time, Eigen::Affine3f & out_transform);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;

};

inline double TransformAnimation::GetDuration() const
{
	return duration;
}
inline double TransformAnimation::GetTicksPerSecond() const
{
	return ticks_per_second;
}
inline TransformAnimation::Behavior TransformAnimation::GetPreBehavior() const
{
	return pre_behavior;
}
inline TransformAnimation::Behavior TransformAnimation::GetPostBehavior() const
{
	return post_behavior;
}

inline void TransformAnimation::SetDuration(double in_duration)
{
	duration = in_duration;
}
inline void TransformAnimation::SetTicksPerSecond(double in_ticks_per_second)
{
	ticks_per_second = in_ticks_per_second;
}
inline void TransformAnimation::SetPreBehavior(Behavior in_pre_behavior)
{
	pre_behavior = in_pre_behavior;
}
inline void TransformAnimation::SetPostBehavior(Behavior in_post_behavior)
{
	post_behavior = in_post_behavior;
}



#endif
*/