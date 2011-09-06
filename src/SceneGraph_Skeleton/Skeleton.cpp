#include "Skeleton.h"
#include <iostream>

Skeleton::Skeleton()
{

}
Skeleton::~Skeleton()
{

}

void Skeleton::PrintInverseBind() const
{

}
void Skeleton::PrintDefaultPose() const
{
	for (unsigned int i = 0; i < joints.size(); ++i)
	{
		std::cout << joints[i].GetDefaultPose().matrix() << std::endl << std::endl;
	}
}

//Serialization
void Skeleton::Serialize(Stream & stream) const
{
	STLSerializer<std::vector<Joint, Eigen::aligned_allocator<Joint> > > joints_serializer(stream);
	joints_serializer.Save(joints);
}
void Skeleton::Unserialize(Stream & stream)
{
	STLSerializer<std::vector<Joint, Eigen::aligned_allocator<Joint> > > joints_serializer(stream);
	joints_serializer.Load(joints);
}
void Skeleton::SerializeTypeId(Stream & stream) const
{
	stream.Save("Skeleton");
}