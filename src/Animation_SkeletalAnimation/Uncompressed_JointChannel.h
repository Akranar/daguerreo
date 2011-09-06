#ifndef _JOINT_CHANNEL_H_
#define _JOINT_CHANNEL_H_

#include <vector>
#include "ExLib_Eigen.h"
#include "ExLib_Serialization.h"
#include "TranslationKey.h"
#include "ScaleKey.h"
#include "RotationKey.h"

class Uncompressed_JointChannel: public Serializable
{
	unsigned char joint_index;
	unsigned int pre_behavior;
	unsigned int post_behavior;
	unsigned int translation_keys_count;
	unsigned int scale_keys_count;
	unsigned int rotation_keys_count;

	TranslationKey * translation_keys;
	ScaleKey * scale_keys;
	RotationKey * rotation_keys;
public:
	Uncompressed_JointChannel();
	Uncompressed_JointChannel(unsigned char joint_index, unsigned int scale_keys_count, unsigned int rotation_keys_count, unsigned int translation_keys_count);
	~Uncompressed_JointChannel();

	inline unsigned char GetJointIndex() const;
	inline void SetScale(unsigned int index, const ScaleKey & key);
	inline void SetTranslation(unsigned int index, const TranslationKey & key);
	inline void SetRotation(unsigned int index, const RotationKey & key);
	inline void SetBehavior(unsigned int in_pre_behavior, unsigned int in_post_behavior);
	inline void GetTransformAt_SLERP(double time, Eigen::Affine3f & transform, unsigned int & s, unsigned int & r, unsigned int & t);

	void InitJointChannel(unsigned char joint_index, unsigned int scale_keys_count, unsigned int rotation_keys_count, unsigned int translation_keys_count);

	void PrintScaleKeys() const;
	void PrintRotationKeys() const;
	void PrintTranslationKeys() const;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline unsigned char Uncompressed_JointChannel::GetJointIndex() const
{
	return joint_index;
}

inline void Uncompressed_JointChannel::SetScale(unsigned int index, const ScaleKey & key)
{
	scale_keys[index] = key;
}
inline void Uncompressed_JointChannel::SetTranslation(unsigned int index, const TranslationKey & key)
{
	translation_keys[index] = key;
}
inline void Uncompressed_JointChannel::SetRotation(unsigned int index, const RotationKey & key)
{
	rotation_keys[index] = key;
}

inline void Uncompressed_JointChannel::SetBehavior(unsigned int in_pre_behavior, unsigned int in_post_behavior)
{
	pre_behavior = in_pre_behavior;
	post_behavior = in_post_behavior;
}

inline void Uncompressed_JointChannel::GetTransformAt_SLERP(double time, Eigen::Affine3f & transform, unsigned int & s, unsigned int & r, unsigned int & t)
{
	Eigen::Vector3f scale;
	Eigen::Quaternionf rotation;
	Eigen::Vector3f translation;

	if (scale_keys_count > 1)
	{
		for (; s < scale_keys_count; ++s)
		{
			if (time < scale_keys[s].GetTime()) break;
		}

		double scale_time = scale_keys[s].GetTime() - scale_keys[s-1].GetTime();
		double time_over_scale_time = (time - scale_keys[s-1].GetTime())/ scale_time;
		scale =  scale_keys[s].GetScale() * (1.0 - time_over_scale_time) + scale_keys[s-1].GetScale() * time_over_scale_time;
	}
	else
	{
		scale = scale_keys[0].GetScale();
	}

	if (rotation_keys_count > 1)
	{
		for (; r < rotation_keys_count; ++r)
		{
			double the_time = rotation_keys[r].GetTime();
			if (time < rotation_keys[r].GetTime()) break;
		}

		Eigen::Quaternionf prev(rotation_keys[r].GetRotation());
		Eigen::Quaternionf post(rotation_keys[r].GetRotation());
		double rot_time = rotation_keys[r].GetTime() - rotation_keys[r-1].GetTime();
		double time_over_rot_time = (time - rotation_keys[r-1].GetTime()) / rot_time;
		rotation = prev.slerp(time_over_rot_time, post);
	}
	else
	{
		rotation = Eigen::Quaternionf(rotation_keys[0].GetRotation());
	}

	if (translation_keys_count > 1)
	{
		for (; t < translation_keys_count; ++t)
		{
			if (time < translation_keys[t].GetTime()) break;
		}
		double trans_time = translation_keys[t].GetTime() - translation_keys[t-1].GetTime();
		double time_over_trans_time = (time - translation_keys[t-1].GetTime()) / trans_time;
		translation =  translation_keys[t].GetTranslation() * (1.0 - time_over_trans_time) + translation_keys[t-1].GetTranslation() * time_over_trans_time;
	}
	else
	{
		translation =  translation_keys[0].GetTranslation();
	}

	transform.linear().noalias() =  rotation.matrix() * scale.asDiagonal();
	transform.translation() = translation;
}


#endif