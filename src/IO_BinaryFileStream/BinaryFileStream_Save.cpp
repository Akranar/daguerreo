#include "BinaryFileStream.h"

bool BinaryFileStream::Save(const Serializable * val)
{
	endian.WriteIntType(stream, val);
	if (ShouldSerialize(val))
	{
		val->SerializeTypeId(*this);
		Save(*val);
	}
	return true;
}

bool BinaryFileStream::Save(const char * val, int length)
{
	//If Null
	if (!val)
	{
		if (!endian.WriteIntType(stream, 0)) return false;
		return stream.good();
	}
	//Else
	else
	{
		unsigned int string_length = strlen(val);
		if (!endian.WriteIntType(stream, string_length)) return false;
		for (unsigned int i = 0; i < string_length; ++i)
		{
			stream.put(val[i]);
		}
		return stream.good();
	}
}

bool BinaryFileStream::Save(const unsigned int & val)
{
	endian.WriteIntType(stream, val);
	return true;
}
bool BinaryFileStream::Save(const unsigned char & val)
{
	endian.WriteIntType(stream, val);
	return true;
}
bool BinaryFileStream::Save(const unsigned short & val)
{
	endian.WriteIntType(stream, val);
	return true;
}
bool BinaryFileStream::Save(const unsigned long & val)
{
	endian.WriteIntType(stream, val);
	return true;
}


bool BinaryFileStream::Save(const int & val)
{
	endian.WriteIntType(stream, val);
	return true;
}
bool BinaryFileStream::Save(const float & val)
{
	endian.WriteIntType(stream, val);
	return true;
}
bool BinaryFileStream::Save(const double & val)
{
	endian.WriteIntType(stream, val);
	return true;
}
bool BinaryFileStream::Save(const char & val)
{
	endian.WriteIntType(stream, val);
	return true;
}

bool BinaryFileStream::Save(const bool & val)
{
	endian.WriteIntType(stream, val);
	return true;
}

bool BinaryFileStream::Save(const short & val)
{
	endian.WriteIntType(stream, val);
	return true;
}
bool BinaryFileStream::Save(const long & val)
{
	endian.WriteIntType(stream, val);
	return true;
}

bool BinaryFileStream::Save(const Serializable & val)
{
	val.Serialize(*this);
	return true;
}

bool BinaryFileStream::Save(const long double & val)
{
	endian.WriteIntType(stream, val);
	return true;
}