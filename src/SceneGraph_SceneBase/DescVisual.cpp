#include "DescVisual.h"
#include "ExLib_BoundingVolume.h"
#include "ExLib_BoundCreation.h"
#include "ExLib_FloatDataAccessor.h"

DescVisual::~DescVisual()
{
	delete bound;
}

void DescVisual::CreateBoundingVolume(unsigned int type, unsigned char * position_data, unsigned int vertex_stride, unsigned int vertex_count)
{
	FloatDataAccessor float_data((float *) position_data, 3, vertex_count, vertex_stride);
	
	switch(type)
	{
	case BoundingVolume::BV_AABB:
		bound = BoundCreation::NewAABB(float_data);
		break;
	case BoundingVolume::BV_SPHERE:
		bound = BoundCreation::NewSphere(float_data);
		break;
	case BoundingVolume::BV_OBB:
		bound = BoundCreation::NewOBB(float_data);
		break;
	default:
		bound = 0;
		break;
	}
	
}

//Serialization
void DescVisual::Serialize(Stream & stream) const
{
	STLSerializer<std::string> string_serializer(stream);
	string_serializer.Save(vertex_buffer_id);
	string_serializer.Save(index_buffer_id);
	string_serializer.Save(visual_fx_instance_id);
	stream.Save(bound);
}
void DescVisual::SerializeTypeId(Stream & stream) const
{
	stream.Save("DescVisual");
}
void DescVisual::Unserialize(Stream & stream)
{
	STLSerializer<std::string> string_serializer(stream);
	string_serializer.Load(vertex_buffer_id);
	string_serializer.Load(index_buffer_id);
	string_serializer.Load(visual_fx_instance_id);
	stream.Load(bound);
}
