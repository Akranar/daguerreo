#include "Stream.h"





bool Stream::Save(const int * val, int length)
{
	DataIO::WriteInt(*this, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool Stream::Save(const float * val, int length)
{
	DataIO::WriteInt(*this, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool Stream::Save(const double * val, int length)
{
	DataIO::WriteInt(*this, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}


bool Stream::Save(const bool * val, int length)
{
	DataIO::WriteInt(*this, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}

bool Stream::Save(const short * val, int length)
{
	DataIO::WriteInt(*this, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}
bool Stream::Save(const long * val, int length)
{	
	DataIO::WriteInt(*this, length);
	for (int i = 0; i < length; ++i)
	{
		Save(val[i]);
	}
	return true;
}


bool Stream::Save(const int ** val, int level, int * lengths)
{

	return true;
}
bool Stream::Save(const float ** val, int length, int * lengths)
{

	return true;
}
bool Stream::Save(const double ** val, int length, int * lengths)
{

	return true;
}
bool Stream::Save(const char ** val, int length)
{

	return true;
}

bool Stream::Save(const bool ** val, int length, int * lengths)
{

	return true;
}

bool Stream::Save(const short ** val, int length, int * lengths)
{

	return true;
}
bool Stream::Save(const long ** val, int length, int * lengths)
{

	return true;
}
bool Stream::Save(const Serializable ** val, int length, int * lengths)
{

	return true;
}




