#include "Stream.h"
#include "ExLib_TestingTools.h"

/*
const std::vector<Serializable *> & Stream::GetLoadedObjects() const
{
	return loaded_objects;
}
*/
/*
void Stream::Close()
{
	this->object_id_map.clear();
	this->serialization_queue.clear();

	for (unsigned int i = current_index; i < loaded_objects.size(); ++i)
	{
		delete loaded_objects[i];
	}
	this->loaded_objects.clear();
	this->remap_queue.clear();
}

*/



/*
	Helper Methods for Serializing Nested Objects. {
*/

/*
void Stream::NestedObject_Save(const Serializable & nested_object)
{
	ReserveId(nested_object);
}
void Stream::NestedObject_Serialize(const Serializable & nested_object)
{
	int nested_object_id = GetId(&nested_object);
	DataIO::WriteInt(*this, nested_object_id);
	nested_object.Serialize(*this);
}
void Stream::NestedObject_Unserialize(Serializable & nested_object, const char * type_name)
{
	int nested_object_id = DataIO::ReadInt(*this);
	const char * nested_object_type = DataIO::ReadString(*this);
	Assert(strcmp(nested_object_type, type_name) == 0, "Object Type Mismatch");
	delete nested_object_type;
	nested_object.Unserialize(*this);
	QueueRemapReservedId(nested_object_id, nested_object);
}
void Stream::NestedObject_FixUp(Serializable & nested_object)
{
	nested_object.FixUp(*this);
}

*/
/*
	Helper Methods for Serializing Nested Objects. }
	Helper Methods for Serializing Pointers. {
*/

/*
void Stream::Pointer_Save(const Serializable * pointer_object)
{
	if (pointer_object) pointer_object->Save(*this);
}
void Stream::Pointer_Serialize(const Serializable * pointer_object)
{
	DataIO::WriteInt(*this, GetId(pointer_object));
}
*/

/*
	Helper Methods for Serializing Pointers. }
	Stream Reading Methods {
*/
/*
Serializable * Stream::GetNext()
{
	Serializable * rtn = loaded_objects[current_index];
	current_index += 1;
	return rtn;
}
bool Stream::EndOfFile()
{
	return current_index >= loaded_objects.size();
}
void Stream::SetEOF()
{
	current_index = loaded_objects.size();
}
*/
/*
	Stream Reading Methods }
*/