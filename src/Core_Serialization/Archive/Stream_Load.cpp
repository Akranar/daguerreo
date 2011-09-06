#include "Stream.h"
#include "ExLib_ObjectCreation.h"



bool Stream::Load(int *& val, int length)
{
	int len = DataIO::ReadInt(*this);
	val = new int[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool Stream::Load(float *& val, int length)
{
	int len = DataIO::ReadInt(*this);
	val = new float[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool Stream::Load(double *& val, int length)
{
	int len = DataIO::ReadInt(*this);
	val = new double[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}


bool Stream::Load(bool *& val, int length)
{
	int len = DataIO::ReadInt(*this);
	val = new bool[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}

bool Stream::Load(short *& val, int length)
{
	int len = DataIO::ReadInt(*this);
	val = new short[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}
bool Stream::Load(long *& val, int length)
{
	int len = DataIO::ReadInt(*this);
	val = new long[len];
	for (int i = 0; i < len; ++i)
	{
		Load(val[i]);
	}
	return true;
}


bool Stream::Load(int **& val, int level, int * lengths)
{
	
	return true;
}
bool Stream::Load(float **& val, int length, int * lengths)
{
	
	return true;
}
bool Stream::Load(double **& val, int length, int * lengths)
{
	
	return true;
}
bool Stream::Load(char **& val, int length, int * lengths)
{
	
	return true;
}
bool Stream::Load(bool **& val, int length, int * lengths)
{
	
	return true;
}
bool Stream::Load(short **& val, int length, int * lengths)
{
	
	return true;
}
bool Stream::Load(long **& val, int length, int * lengths)
{
	
	return true;
}


bool Stream::Load(Serializable **& val, int length, int * lengths)
{
	
	return true;
}
