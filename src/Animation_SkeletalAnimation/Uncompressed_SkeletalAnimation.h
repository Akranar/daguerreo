#ifndef _UNCOMPRESSED_SKELETAL_ANIMATION_H_
#define _UNCOMPRESSED_SKELETAL_ANIMATION_H_

#include "SkeletalAnimation.h"
#include "Uncompressed_JointChannel.h"

class SkinnedMesh;

class Uncompressed_SkeletalAnimation: public SkeletalAnimation
{
	double duration_in_seconds;
	unsigned int joint_channel_count;
	Uncompressed_JointChannel * joint_channels;
public:
	Uncompressed_SkeletalAnimation();
	Uncompressed_SkeletalAnimation(double duration, unsigned int joint_channel_count);
	virtual ~Uncompressed_SkeletalAnimation();

	inline Uncompressed_JointChannel & GetJointChannel(unsigned int index);
	virtual void UpdateSkinnedMesh(SkinnedMesh * skinned_mesh, double time, float blend_factor, bool overwrite);

	void Print() const;
	virtual double GetDuration() const;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;

};



inline Uncompressed_JointChannel & Uncompressed_SkeletalAnimation::GetJointChannel(unsigned int index)
{
	return joint_channels[index];
}

#endif