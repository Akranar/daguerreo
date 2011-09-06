#include "Uncompressed_JointChannel.h"
#include <iostream>

Uncompressed_JointChannel::Uncompressed_JointChannel()
:
joint_index(-1),
pre_behavior(-1),
post_behavior(-1),
translation_keys_count(0),
scale_keys_count(0),
rotation_keys_count(0),
translation_keys(0),
scale_keys(0),
rotation_keys(0)
{

}
Uncompressed_JointChannel::Uncompressed_JointChannel(unsigned char joint_index, unsigned int scale_keys_count, unsigned int rotation_keys_count, unsigned int translation_keys_count)
:
joint_index(joint_index),
pre_behavior(-1),
post_behavior(-1),
translation_keys_count(translation_keys_count),
scale_keys_count(scale_keys_count),
rotation_keys_count(rotation_keys_count),
translation_keys(new TranslationKey[translation_keys_count]),
scale_keys(new ScaleKey[scale_keys_count]),
rotation_keys(new RotationKey[rotation_keys_count])
{

}

void Uncompressed_JointChannel::InitJointChannel(unsigned char in_joint_index, 
												 unsigned int in_scale_keys_count, 
												 unsigned int in_rotation_keys_count, 
												 unsigned int in_translation_keys_count)
{
	joint_index = in_joint_index;
	translation_keys_count = in_translation_keys_count;
	scale_keys_count = in_scale_keys_count;
	rotation_keys_count = in_rotation_keys_count;

	if (translation_keys) delete [] translation_keys;
	if (scale_keys) delete [] scale_keys;
	if (rotation_keys) delete [] rotation_keys;

	translation_keys = new TranslationKey[translation_keys_count];
	scale_keys = new ScaleKey[scale_keys_count];
	rotation_keys = new RotationKey[rotation_keys_count];
}


Uncompressed_JointChannel::~Uncompressed_JointChannel()
{
	if (translation_keys) delete [] translation_keys;
	if (scale_keys) delete [] scale_keys;
	if (rotation_keys) delete [] rotation_keys;
}

void Uncompressed_JointChannel::PrintScaleKeys() const
{
	for (unsigned int i = 0; i < scale_keys_count; ++i)
	{
		Eigen::Vector3f scale = scale_keys[i].GetScale();
		std::cout << scale_keys[i].GetTime() << ": " << scale[0] << ", " << scale[1] << ", " << scale[2] << std::endl;
	}
}
void Uncompressed_JointChannel::PrintRotationKeys() const
{
	for (unsigned int i = 0; i < rotation_keys_count; ++i)
	{
		Eigen::Vector4f rot(rotation_keys[i].GetRotation());
		std::cout << rotation_keys[i].GetTime() << ": " << rot[0] << ", " << rot[1] << ", " << rot[2] << rot[3] << std::endl;
	}
}
void Uncompressed_JointChannel::PrintTranslationKeys() const
{
	for (unsigned int i = 0; i < translation_keys_count; ++i)
	{
		Eigen::Vector3f trans = translation_keys[i].GetTranslation();
		std::cout << translation_keys[i].GetTime() << ": " << trans[0] << ", " << trans[1] << ", " << trans[2] << std::endl;
	}
}



void Uncompressed_JointChannel::Serialize(Stream & stream) const
{
	stream.Save(joint_index);
	stream.Save(pre_behavior);
	stream.Save(post_behavior);
	stream.Save(translation_keys_count);
	stream.Save(scale_keys_count);
	stream.Save(rotation_keys_count);

	for (unsigned int i = 0; i < translation_keys_count; ++ i)
	{
		stream.Save(translation_keys[i].GetTime());
		Eigen::Vector3f trans = translation_keys[i].GetTranslation();
		stream.Save(trans.x());
		stream.Save(trans.y());
		stream.Save(trans.z());
	}
	for (unsigned int i = 0; i < scale_keys_count; ++ i)
	{
		stream.Save(scale_keys[i].GetTime());
		Eigen::Vector3f scale = scale_keys[i].GetScale();
		stream.Save(scale.x());
		stream.Save(scale.y());
		stream.Save(scale.z());
	}
	for (unsigned int i = 0; i < rotation_keys_count; ++ i)
	{
		stream.Save(rotation_keys[i].GetTime());
		const float * rot = rotation_keys[i].GetRotation();
		stream.Save(rot[0]);
		stream.Save(rot[1]);
		stream.Save(rot[2]);
		stream.Save(rot[3]);
	}
}


void Uncompressed_JointChannel::Unserialize(Stream & stream)
{
	stream.Load(joint_index);
	stream.Load(pre_behavior);
	stream.Load(post_behavior);
	stream.Load(translation_keys_count);
	stream.Load(scale_keys_count);
	stream.Load(rotation_keys_count);

	translation_keys = new TranslationKey[translation_keys_count];
	scale_keys = new ScaleKey[scale_keys_count];
	rotation_keys = new RotationKey[rotation_keys_count];

	for (unsigned int i = 0; i < translation_keys_count; ++ i)
	{
		double time;
		float trans[3];
		stream.Load(time);
		stream.Load(trans[0]);
		stream.Load(trans[1]);
		stream.Load(trans[2]);
		translation_keys[i].SetTime(time);
		translation_keys[i].SetTranslation(trans);
	}
	for (unsigned int i = 0; i < scale_keys_count; ++ i)
	{
		double time;
		float scale[3];
		stream.Load(time);
		stream.Load(scale[0]);
		stream.Load(scale[1]);
		stream.Load(scale[2]);
		scale_keys[i].SetTime(time);
		scale_keys[i].SetScale(scale);
	}
	for (unsigned int i = 0; i < rotation_keys_count; ++ i)
	{
		double time;
		float rot[4];
		stream.Load(time);
		stream.Load(rot[0]);
		stream.Load(rot[1]);
		stream.Load(rot[2]);
		stream.Load(rot[3]);
		rotation_keys[i].SetTime(time);
		rotation_keys[i].SetRotation(rot);
	}
}
void Uncompressed_JointChannel::SerializeTypeId(Stream & stream) const
{
	stream.Save("Anim_UJC");
}