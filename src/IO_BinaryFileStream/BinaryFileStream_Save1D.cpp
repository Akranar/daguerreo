#include "BinaryFileStream.h"

bool BinaryFileStream::Save(const unsigned char * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}



bool BinaryFileStream::Save(const char ** val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}

bool BinaryFileStream::Save(const unsigned int * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}

bool BinaryFileStream::Save(const unsigned short * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const unsigned long * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const int * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const float * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const double * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const short * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const long * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const long double * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool BinaryFileStream::Save(const bool * val, int length)
{
	endian.WriteIntType(stream, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}