/*
#include "TransformAnimation.h"

TransformAnimation::TransformAnimation()
{
	
}
TransformAnimation::~TransformAnimation()
{

}

void TransformAnimation::GetInterpolatedTransform(double time, Eigen::Affine3f & out_transform)
{
	out_transform = Eigen::Affine3f::Identity();
	for (unsigned int i = 0; i < scale_keys.size(); ++i)
	{
		if (i < scale_keys.size()-1 && scale_keys[i].GetTime() < time && scale_keys[i+1].GetTime() > time)
		{
			out_transform.scale(scale_keys[i].GetScale());
			break;
		}
	}

	for (unsigned int i = 0; i < rotation_keys.size(); ++i)
	{
		if (i < scale_keys.size()-1 && rotation_keys[i].GetTime() < time && rotation_keys[i+1].GetTime() > time)
		{
			out_transform.rotate(rotation_keys[i].GetRotation());
			break;
		}
	}

	for (unsigned int i = 0; i < translation_keys.size(); ++i)
	{
		if (i < translation_keys.size()-1 && translation_keys[i].GetTime() < time && translation_keys[i+1].GetTime() > time)
		{
			out_transform.translate(translation_keys[i].GetTranslation());
			break;
		}
	}
}

void TransformAnimation::Serialize(Stream & stream) const
{
	stream.Save(duration);
	stream.Save(ticks_per_second);
	stream.Save((int)pre_behavior);
	stream.Save((int)post_behavior);

	//STLSerializer<std::vector<TransformKey, Eigen::aligned_allocator<TransformKey> > > transform_serializer(stream);
	//transform_serializer.Save(keys);


	STLSerializer<std::vector<ScaleKey> > scale_serializer(stream);
	STLSerializer<std::vector<RotationKey, Eigen::aligned_allocator<RotationKey> > > rotation_serializer(stream);
	STLSerializer<std::vector<TranslationKey> > translation_serializer(stream);
	scale_serializer.Save(scale_keys);
	rotation_serializer.Save(rotation_keys);
	translation_serializer.Save(translation_keys);

}
void TransformAnimation::Unserialize(Stream & stream)
{
	int pre_behavior_int;
	int post_behavior_int;

	stream.Load(duration);
	stream.Load(ticks_per_second);
	stream.Load(pre_behavior_int);
	stream.Load(post_behavior_int);

	pre_behavior = (Behavior) pre_behavior_int;
	post_behavior = (Behavior) post_behavior_int;

	//STLSerializer<std::vector<TransformKey, Eigen::aligned_allocator<TransformKey> > > transform_serializer(stream);
	//transform_serializer.Load(keys);

	STLSerializer<std::vector<ScaleKey> > scale_serializer(stream);
	STLSerializer<std::vector<RotationKey, Eigen::aligned_allocator<RotationKey> > > rotation_serializer(stream);
	STLSerializer<std::vector<TranslationKey> > translation_serializer(stream);
	scale_serializer.Load(scale_keys);
	rotation_serializer.Load(rotation_keys);
	translation_serializer.Load(translation_keys);
}
void TransformAnimation::SerializeTypeId(Stream & stream) const
{
	stream.Save("TransformAnimation");
}
*/