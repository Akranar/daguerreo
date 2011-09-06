#include "BinaryFileStream.h"
#include <Core_ObjectFactory.h>

bool BinaryFileStream::Load(Serializable *& val)
{
	const Serializable * old_address = (const Serializable *) endian.ReadIntType<void*>(stream);
	if (old_address == NULL)
	{
		val = NULL;
		return true;
	}
	if (ShouldUnserialize(old_address, val))
	{
		char * object_type_id;
		Load(object_type_id);
		val = load_factory->Create(object_type_id);
		delete object_type_id;
		SetOldToNew(old_address, val);
		val->Unserialize(*this);
		
	}
	return true;
}

bool BinaryFileStream::Load(char *& val, int)
{
	unsigned int string_length = endian.ReadIntType<unsigned int>(stream);
	char * rtn = new char[string_length + 1];
	rtn[string_length] = 0;
	for (unsigned int i = 0; i < string_length; ++i)
	{
		rtn[i] = stream.get();
	}
	val = rtn;
	return true;
}

bool BinaryFileStream::Load(Serializable & val)
{
	val.Unserialize(*this);
	return true;
}

bool BinaryFileStream::Load(unsigned int & val)
{
	val = endian.ReadIntType<unsigned int>(stream);
	return true;
}
bool BinaryFileStream::Load(unsigned char & val)
{
	val = endian.ReadIntType<unsigned char>(stream);
	return true;
}
bool BinaryFileStream::Load(unsigned short & val)
{
	val = endian.ReadIntType<unsigned short>(stream);
	return true;
}
bool BinaryFileStream::Load(unsigned long & val)
{
	val = endian.ReadIntType<unsigned long>(stream);
	return true;
}


bool BinaryFileStream::Load(int & val)
{
	val = endian.ReadIntType<int>(stream);
	return true;
}
bool BinaryFileStream::Load(float & val)
{
	val = endian.ReadIntType<float>(stream);
	return true;
}
bool BinaryFileStream::Load(double & val)
{
	val = endian.ReadIntType<double>(stream);
	return true;
}
bool BinaryFileStream::Load(char & val)
{
	val = endian.ReadIntType<char>(stream);
	return true;
}

bool BinaryFileStream::Load(bool & val)
{
	val = endian.ReadIntType<bool>(stream);
	return true;
}

bool BinaryFileStream::Load(short & val)
{
	val = endian.ReadIntType<short>(stream);
	return true;
}
bool BinaryFileStream::Load(long & val)
{
	val = endian.ReadIntType<long>(stream);
	return true;
}

bool BinaryFileStream::Load(long double & val)
{
	val = endian.ReadIntType<long double>(stream);
	return true;
}

