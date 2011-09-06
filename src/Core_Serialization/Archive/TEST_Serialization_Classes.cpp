#include "TEST_Serialization.h"
#include "Stream.h"
#include "DataIO.h"
#include "ExLib_TestingTools.h"

void ObjectA::SerializeTypeId(Stream & stream) const
{

}

void ObjectB::SerializeTypeId(Stream & stream) const
{

}


void ObjectC::SerializeTypeId(Stream & stream) const
{

}


void ObjectD::SerializeTypeId(Stream & stream) const
{

}



void ObjectA::Print() const
{
	std::cout << "ObjectA: " << std::endl;
	std::cout << "	Data0: " << data0 << std::endl;
	std::cout << "	Data1: " << data1 << std::endl;
	std::cout << "	Data2: " << data2 << std::endl;
	std::cout << "	Data3: " << data3 << std::endl;
}

void ObjectA::Save(Stream & stream) const
{
	stream.QueueForSerialization(this);
}

void ObjectA::Serialize(Stream & stream) const
{
	DataIO::WriteString(stream, "ObjectA");

	DataIO::WriteInt(stream, data0);
	DataIO::WriteFloat(stream, data1);
	DataIO::WriteDouble(stream, data2);
	DataIO::WriteString(stream, data3.c_str());
}
void ObjectA::Unserialize(Stream & stream)
{
	data0 = DataIO::ReadInt(stream);
	data1 = DataIO::ReadFloat(stream);
	data2 = DataIO::ReadDouble(stream);
	const char * data3_temp = DataIO::ReadString(stream);
	data3.assign(data3_temp);
	delete data3_temp;
}
void ObjectA::FixUp(Stream & stream)
{
	
}

void ObjectB::Print() const
{
	std::cout << "ObjectB: " << std::endl;
	std::cout << "	Data0: " << data0 << std::endl;
	std::cout << "	Data1: " << data1 << std::endl;
	std::cout << "	Data2: " << data2 << std::endl;
	std::cout << "	Data3: " << data3 << std::endl;
	std::cout << "	Data4: " << data4 << std::endl;
	if (data4) std::cout << "		Data3: " << data4->data3 << std::endl;
}

void ObjectB::Save(Stream & stream) const
{
	if (stream.QueueForSerialization(this))
	{
		if (data4) data4->Save(stream);
	}
}

void ObjectB::Serialize(Stream & stream) const
{
	DataIO::WriteString(stream, "ObjectB");

	DataIO::WriteInt(stream, data0);
	DataIO::WriteFloat(stream, data1);
	DataIO::WriteDouble(stream, data2);
	DataIO::WriteString(stream, data3.c_str());
	DataIO::WriteInt(stream, stream.GetId(data4));
}
void ObjectB::Unserialize(Stream & stream)
{
	data0 = DataIO::ReadInt(stream);
	data1 = DataIO::ReadFloat(stream);
	data2 = DataIO::ReadDouble(stream);
	const char * data3_temp = DataIO::ReadString(stream);
	data3.assign(data3_temp);
	delete data3_temp;
	data4 = (ObjectC *) DataIO::ReadInt(stream);
}
void ObjectB::FixUp(Stream & stream)
{
	data4 = (ObjectC *) stream.GetObjectForFixUp((unsigned int)data4);
}

void ObjectC::Print() const
{
	std::cout << "ObjectC: " << std::endl;
	std::cout << "	Data0: " << data0 << std::endl;
	std::cout << "	Data1: " << data1 << std::endl;
	std::cout << "	Data2: " << data2 << std::endl;
	std::cout << "	Data3: " << data3 << std::endl;
	std::cout << "	Data4: " << data4 << std::endl;
	if (data4) std::cout << "		Data3: " << data4->data3 << std::endl;
}

void ObjectC::Save(Stream & stream) const
{
	if (stream.QueueForSerialization(this))
	{
		//if (data4) data4->Save(stream);
		stream.Pointer_Save(data4);
	}
}

void ObjectC::Serialize(Stream & stream) const
{
	DataIO::WriteString(stream, "ObjectC");

	DataIO::WriteInt(stream, data0);
	DataIO::WriteFloat(stream, data1);
	DataIO::WriteDouble(stream, data2);
	DataIO::WriteString(stream, data3.c_str());
	//DataIO::WriteInt(stream, stream.GetId(data4));
	stream.Pointer_Serialize(data4);
}
void ObjectC::Unserialize(Stream & stream)
{
	data0 = DataIO::ReadInt(stream);
	data1 = DataIO::ReadFloat(stream);
	data2 = DataIO::ReadDouble(stream);
	const char * data3_temp = DataIO::ReadString(stream);
	data3.assign(data3_temp);
	delete data3_temp;
	//data4 = (ObjectB *) DataIO::ReadInt(stream);
	data4 = stream.Pointer_Unserialize<ObjectB*>(data4);
}
void ObjectC::FixUp(Stream & stream)
{
	//data4 = (ObjectB *) stream.GetObjectForFixUp((unsigned int)data4);
	data4 = stream.Pointer_FixUp<ObjectB*>(data4);
}

void ObjectD::Print() const
{
	std::cout << "ObjectD: " << std::endl;
	std::cout << "	Data0: " << data0 << std::endl;
	std::cout << "	Data1: " << data1 << std::endl;
	std::cout << "	Data2: " << data2 << std::endl;
	std::cout << "	Data3: " << data3 << std::endl;
	object_a.Print();
}

void ObjectD::Save(Stream & stream) const
{
	if(stream.QueueForSerialization(this))
	{
		/*
		stream.ReserveId(object_a);
		*/
		stream.NestedObject_Save(object_a);
	}
}

void ObjectD::Serialize(Stream & stream) const
{
	DataIO::WriteString(stream, "ObjectD");

	DataIO::WriteInt(stream, data0);
	DataIO::WriteFloat(stream, data1);
	DataIO::WriteDouble(stream, data2);
	DataIO::WriteString(stream, data3.c_str());

	/*
	int object_a_id = stream.GetId(&object_a);
	DataIO::WriteInt(stream, object_a_id);
	object_a.Serialize(stream);
	*/
	stream.NestedObject_Serialize(object_a);
}
void ObjectD::Unserialize(Stream & stream)
{
	data0 = DataIO::ReadInt(stream);
	data1 = DataIO::ReadFloat(stream);
	data2 = DataIO::ReadDouble(stream);
	const char * data3_temp = DataIO::ReadString(stream);
	data3.assign(data3_temp);
	delete data3_temp;

	/*
	int object_a_id = DataIO::ReadInt(stream);
	const char * object_a_type = DataIO::ReadString(stream);
	Assert(strcmp(object_a_type, "ObjectA") == 0, "Object Mismatch. Expected ObjectA");
	delete object_a_type;
	object_a.Unserialize(stream);
	stream.QueueRemapReservedId(object_a_id, object_a);
	*/
	stream.NestedObject_Unserialize(object_a, "ObjectA");
}
void ObjectD::FixUp(Stream & stream)
{
	/*
	object_a.FixUp(stream);
	*/
	stream.NestedObject_FixUp(object_a);
}



