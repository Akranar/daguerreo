#ifndef _STL_SERIALIZER_H_
#define _STL_SERIALIZER_H_

#include "Stream.h"
#include <vector>
#include <set>
#include <map>
#include <string>
#include <Core_FixedArray.h>

template<typename TYPE>
class STLSerializer
{
	Stream & stream;
public:
	STLSerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~STLSerializer()
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

template<>
class STLSerializer<std::string>
{
	Stream & stream;
public:
	STLSerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~STLSerializer()
	{
	}
	void Save(const std::string & object)
	{
		stream.Save(object.c_str());
	}
	void Load(std::string & object)
	{
		const char * object_cstr;
		stream.Load(object_cstr);
		object.assign(object_cstr);
		delete object_cstr;
	}
};

template<typename TYPE_A, typename TYPE_B>
class STLSerializer<std::pair<TYPE_A, TYPE_B> >
{
	Stream & stream;
public:
	STLSerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~STLSerializer()
	{
	}
	void Save(const std::pair<TYPE_A, TYPE_B> & object)
	{
		STLSerializer<TYPE_A> typea_serializer(stream);
		STLSerializer<TYPE_B> typeb_serializer(stream);

		typea_serializer.Save(object.first);
		typeb_serializer.Save(object.second);
	}
	void Load(std::pair<TYPE_A, TYPE_B> & object)
	{
		STLSerializer<TYPE_A> typea_serializer(stream);
		STLSerializer<TYPE_B> typeb_serializer(stream);

		typea_serializer.Load(object.first);
		typeb_serializer.Load(object.second);
	}
};

template<typename TYPE, class ALLOCATOR>
class STLSerializer<std::vector<TYPE, ALLOCATOR> >
{
	Stream & stream;
public:
	STLSerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~STLSerializer()
	{
	}
	void Save(const std::vector<TYPE, ALLOCATOR> & object)
	{
		stream.Save(static_cast<unsigned int>(object.size()));
		STLSerializer<TYPE> inner_serializer(stream);
		for (unsigned int i = 0; i < object.size(); ++i)
		{
			inner_serializer.Save(object[i]);
		}
	}
	void Load(std::vector<TYPE, ALLOCATOR> & object)
	{
		unsigned int vector_length;
		stream.Load(vector_length);
		object.reserve(vector_length);
		STLSerializer<TYPE> inner_serializer(stream);
		for (unsigned int i = 0; i < vector_length; ++i)
		{
			TYPE item;
			inner_serializer.Load(item);
			object.push_back(item);
		}
	}
};

template<typename TYPE>
class STLSerializer<FixedArray<TYPE> >
{
	Stream & stream;
public:
	STLSerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~STLSerializer()
	{
	}
	void Save(const FixedArray<TYPE> & object)
	{
		stream.Save(static_cast<unsigned int>(object.GetSize()));
		STLSerializer<TYPE> inner_serializer(stream);
		for (unsigned int i = 0; i < object.GetSize(); ++i)
		{
			inner_serializer.Save(object[i]);
		}
	}
	void Load(FixedArray<TYPE> & object)
	{
		unsigned int vector_length;
		stream.Load(vector_length);
		object.Set(vector_length);
		STLSerializer<TYPE> inner_serializer(stream);
		for (unsigned int i = 0; i < vector_length; ++i)
		{
			inner_serializer.Load(object[i]);
		}
	}
};

template<typename TYPE, class LESS, class ALLOCATOR>
class STLSerializer<std::set<TYPE, LESS, ALLOCATOR> >
{
	Stream & stream;
public:
	STLSerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~STLSerializer()
	{
	}
	void Save(const std::set<TYPE, LESS, ALLOCATOR> & object)
	{
		stream.Save(static_cast<unsigned int>(object.size()));
		STLSerializer<TYPE> inner_serializer(stream);
		for (typename std::set<TYPE, LESS, ALLOCATOR>::const_iterator it = object.begin(); it != object.end(); ++it)
		{
			inner_serializer.Save(*it);
		}
	}
	void Load(std::set<TYPE, LESS, ALLOCATOR> & object)
	{
		unsigned int set_length;
		stream.Load(set_length);
		STLSerializer<TYPE> inner_serializer(stream);
		for (unsigned int i = 0; i < set_length; ++i)
		{
			TYPE item;
			inner_serializer.Load(item);
			object.insert(item);
		}
	}
};


template<typename TYPE_A, typename TYPE_B, class LESS, class ALLOCATOR>
class STLSerializer<std::map<TYPE_A, TYPE_B, LESS, ALLOCATOR> >
{
	Stream & stream;
public:
	STLSerializer(Stream & stream)
		:
	stream(stream)
	{
	}
	~STLSerializer()
	{
	}
	void Save(const std::map<TYPE_A, TYPE_B, LESS, ALLOCATOR> & object)
	{
		stream.Save(static_cast<unsigned int>(object.size()));
		STLSerializer<std::pair<TYPE_A, TYPE_B> > inner_serializer(stream);
		for (typename std::map<TYPE_A, TYPE_B, LESS, ALLOCATOR>::const_iterator it = object.begin(); it != object.end(); ++it)
		{
			inner_serializer.Save(*it);
		}
	}
	void Load(std::map<TYPE_A, TYPE_B, LESS, ALLOCATOR> & object)
	{
		unsigned int set_length;
		stream.Load(set_length);
		STLSerializer<std::pair<TYPE_A, TYPE_B> > inner_serializer(stream);
		for (unsigned int i = 0; i < set_length; ++i)
		{
			std::pair<TYPE_A, TYPE_B> item;
			inner_serializer.Load(item);
			object.insert(item);
		}
	}
};

#endif