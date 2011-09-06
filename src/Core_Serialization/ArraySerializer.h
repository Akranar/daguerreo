#ifndef _ARRAY_SERIALIZER_H_
#define _ARRAY_SERIALIZER_H_

#include "Stream.h"

template<typename TYPE>
class ArraySerializer
{
	Stream & stream;
public:
	ArraySerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~ArraySerializer()
	{
	}
	void Save(const TYPE & item)
	{
		stream.Save(item);
	}
	void Load(TYPE & item)
	{
		stream.Load(item);
	}
};

template<typename TYPE>
class ArraySerializer<TYPE *>
{
	Stream & stream;
public:
	ArraySerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~ArraySerializer()
	{
	}
	void Save(const TYPE * object, unsigned int length)
	{
		stream.Save((unsigned int)length);
		for (unsigned int i = 0; i < length; ++i)
		{
			stream.Save(object[i]);
		}
	}
	void Load(TYPE *& object, unsigned int & length)
	{
		stream.Load((unsigned int &)length);
		if (length > 0)
		{
			object = new TYPE[length];
			for (unsigned int i = 0; i < length; ++i)
			{
				stream.Load(object[i]);
			}
		}
	}
};

#endif