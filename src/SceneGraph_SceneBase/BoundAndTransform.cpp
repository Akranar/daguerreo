#include "BoundAndTransform.h"
#include "ExLib_BoundingVolume.h"

BoundAndTransform::BoundAndTransform()
: 
local_transform(Eigen::Affine3f::Identity()),
world_transform(Eigen::Affine3f::Identity()),
world_bound(0)
{
}
BoundAndTransform::~BoundAndTransform()
{
	if (world_bound) delete world_bound;
}

void BoundAndTransform::Serialize(Stream & stream) const
{
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Save(local_transform.data()[i]);
	}
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Save(world_transform.data()[i]);
	}
}
void BoundAndTransform::Unserialize(Stream & stream)
{
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Load(local_transform.data()[i]);
	}
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Load(world_transform.data()[i]);
	}
	world_bound = 0;
}
void BoundAndTransform::SerializeTypeId(Stream & stream) const
{
	stream.Save("BoundAndTransform");
}