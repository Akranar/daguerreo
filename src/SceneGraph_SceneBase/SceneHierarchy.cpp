#include "SceneHierarchy.h"

SceneHierarchy::~SceneHierarchy()
{
	/*
	for (unsigned int i = 0; i < GetOrder(); ++i)
	{
		delete GetChildAt(i);
	}
	*/
}

void SceneHierarchy::DeleteChildren()
{
	for (unsigned int i = 0; i < GetOrder(); ++i)
	{
		delete GetChildAt(i);
	}
	DetachAll();
}

//Serialization
void SceneHierarchy::Serialize(Stream & stream) const
{
	Spatial::Serialize(stream);
	stream.Save(parent);
	STLSerializer<std::vector<Spatial*> > children_serializer(stream);
	children_serializer.Save(children);
}
void SceneHierarchy::Unserialize(Stream & stream)
{
	Spatial::Unserialize(stream);
	stream.Load(parent);
	STLSerializer<std::vector<Spatial*> > children_serializer(stream);
	children_serializer.Load(children);
}
void SceneHierarchy::SerializeTypeId(Stream & stream) const
{
	stream.Save("SceneHierarchy");
}