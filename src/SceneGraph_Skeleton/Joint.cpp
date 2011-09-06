#include "Joint.h"

Joint::Joint()
:
parent_index(-1)
{
	
}

Joint::Joint(unsigned char parent_index)
:
parent_index(parent_index)
{
	
}

Joint::~Joint()
{

}

//Serialization
void Joint::Serialize(Stream & stream) const
{
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Save(inverse_bind_matrix.data()[i]);
	}
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Save(default_local_pose.data()[i]);
	}
	stream.Save(parent_index);
}
void Joint::Unserialize(Stream & stream)
{
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Load(inverse_bind_matrix.data()[i]);
	}
	for (unsigned int i = 0; i < 16; ++i)
	{
		stream.Load(default_local_pose.data()[i]);
	}
	stream.Load(parent_index);
}
void Joint::SerializeTypeId(Stream & stream) const
{
	stream.Save("Joint");
}