#include "Uncompressed_SkeletalAnimation.h"
#include "Uncompressed_JointChannel.h"
#include "ExLib_Skeleton.h"
#include <iostream>

Uncompressed_SkeletalAnimation::Uncompressed_SkeletalAnimation()
:
duration_in_seconds(0),
joint_channel_count(0),
joint_channels(0)
{

}
Uncompressed_SkeletalAnimation::Uncompressed_SkeletalAnimation(double duration, unsigned int joint_channel_count)
:
duration_in_seconds(duration),
joint_channel_count(joint_channel_count),
joint_channels(new Uncompressed_JointChannel[joint_channel_count])
{

}
Uncompressed_SkeletalAnimation::~Uncompressed_SkeletalAnimation()
{
	if (joint_channels) delete [] joint_channels;
}

double Uncompressed_SkeletalAnimation::GetDuration() const
{
	return duration_in_seconds;
}

void Uncompressed_SkeletalAnimation::UpdateSkinnedMesh(SkinnedMesh * skinned_mesh, double time, float blend_factor, bool overwrite)
{
	//if (overwrite)
	//{
		for (unsigned int i = 0; i < joint_channel_count; ++i)
		{
			unsigned int s = 0;
			unsigned int r = 0;
			unsigned int t = 0;
			joint_channels[i].GetTransformAt_SLERP(time, skinned_mesh->GetLocalPose(joint_channels[i].GetJointIndex()), s, r, t);
			//skinned_mesh->GetLocalPose(i) *= blend_factor;
		}
		/*
	}
	else
	{
		for (unsigned int i = 0; i < joint_channel_count; ++i)
		{
			unsigned int s = 0;
			unsigned int r = 0;
			unsigned int t = 0;
			Eigen::Affine3f temp;
			temp.makeAffine();
			joint_channels[i].GetTransformAt_SLERP(time, temp, s, r, t);
			skinned_mesh->GetLocalPose(i).matrix() += temp.matrix() * blend_factor;
		}
	}
	*/
}

void Uncompressed_SkeletalAnimation::Print() const
{
	joint_channels[0].PrintRotationKeys();
}

void Uncompressed_SkeletalAnimation::Serialize(Stream & stream) const
{
	stream.Save(duration_in_seconds);
	stream.Save(joint_channel_count);
	for (unsigned int i = 0; i < joint_channel_count; ++i)
	{
		stream.Save(joint_channels[i]);
	}
}
void Uncompressed_SkeletalAnimation::Unserialize(Stream & stream)
{
	stream.Load(duration_in_seconds);
	stream.Load(joint_channel_count);
	joint_channels = new Uncompressed_JointChannel[joint_channel_count];
	for (unsigned int i = 0; i < joint_channel_count; ++i)
	{
		stream.Load(joint_channels[i]);
	}
}
void Uncompressed_SkeletalAnimation::SerializeTypeId(Stream & stream) const
{
	stream.Save("Anim_USkelAnim");
}