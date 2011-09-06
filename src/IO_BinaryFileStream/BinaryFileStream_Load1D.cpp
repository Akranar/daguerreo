#include "BinaryFileStream.h"


bool BinaryFileStream::Load(const char **& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new const char *[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}

bool BinaryFileStream::Load(unsigned int *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	if (len > 0)
	{
		val = new unsigned int [len];
		for (int i = 0; i < len; ++i)
		{
			Load(val[i]);
		}
	}
	else
	{
		val = 0;
	}
	return true;
}
bool BinaryFileStream::Load(unsigned short *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new unsigned short[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(unsigned long *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new unsigned long[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(unsigned char *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new unsigned char[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(int *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new int[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(float *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new float[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(double *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new double[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(short *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new short[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(long *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new long[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(long double *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new long double[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool BinaryFileStream::Load(bool *& val, int length)
{
	int len = endian.ReadIntType<int>(stream);
	val = new bool[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}