#include "DescSkinnedMesh.h"

void DescSkinnedMesh::Serialize(Stream & stream) const
{
	STLSerializer<std::string> string_serial(stream);
	STLSerializer<std::vector<std::string> > vector_serial(stream);
	string_serial.Save(skeleton_id);
	vector_serial.Save(visual_ids);
}
void DescSkinnedMesh::Unserialize(Stream & stream)
{
	STLSerializer<std::string> string_serial(stream);
	STLSerializer<std::vector<std::string> > vector_serial(stream);
	string_serial.Load(skeleton_id);
	vector_serial.Load(visual_ids);
}
void DescSkinnedMesh::SerializeTypeId(Stream & stream) const
{
	stream.Save("DescSkinnedMesh");
}