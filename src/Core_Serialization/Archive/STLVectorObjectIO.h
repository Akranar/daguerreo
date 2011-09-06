#ifndef _STL_VECTOR_OBJECT_IO_H_
#define _STL_VECTOR_OBJECT_IO_H_

#include <vector>
#include "Serializable.h"
#include "Stream.h"
#include "DataIO.h"
#include "ExLib_TestingTools.h"

#define TOSTRING(x) #x

template<typename T>
class STLVectorObjectIO : public Serializable
{
	
public:
	typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
	std::vector<const_pointer> * target_vector;
	std::vector<pointer> * loaded_vector;
	std::string value_type_name;
public:

	STLVectorObjectIO<T>(std::vector<const_pointer> * value, const char * value_type_name)
	{
		target_vector = value;
		loaded_vector = 0;
		this->value_type_name.assign(value_type_name);
	}
	STLVectorObjectIO<T>(std::vector<pointer> * value, const char * value_type_name)
	{
		target_vector = reinterpret_cast<std::vector<const_pointer> *> (value);
		loaded_vector = 0;
		this->value_type_name.assign(value_type_name);
	}
	STLVectorObjectIO<T>()
	{
		target_vector = 0;
		loaded_vector = new std::vector<pointer>();
	};
	virtual ~STLVectorObjectIO<T>()
	{
		if (loaded_vector)
		{
			loaded_vector->clear();
			delete loaded_vector;
		}
	};

	std::vector<pointer> * TransferLoadedVector()
	{
		std::vector<pointer> * rtn = loaded_vector;
		loaded_vector = 0;
		return rtn;
	}

	void DeleteObjects()
	{
		unsigned int size = loaded_vector->size();
		for (unsigned int i = 0; i < size; ++i)
		{
			delete (*loaded_vector)[i];
		}
		delete loaded_vector;
		loaded_vector = 0;
	}

	/*
	template<typename U>
	void TransferContent(std::vector<U> & dest)
	{
		dest.clear();
		dest.reserve(loaded_vector->size());
		for(unsigned int i = 0; i < loaded_vector->size(); ++i)
		{
			dest.push_back(reinterpret_cast<U>((*loaded_vector)[i]));
		}
		loaded_vector->clear();
	}
	*/

	void Save(Stream & stream) const;
	void Serialize(Stream & stream) const;
	void Unserialize(Stream & stream);
	void FixUp(Stream & stream);
	void SerializeTypeId(Stream & stream) const {};


	pointer operator[] (unsigned int index)
	{
		Assert(loaded_vector != 0, "Attempting to access NULL vector");
		return (*loaded_vector)[index];
	}

	template<typename GETTYPE>
	GETTYPE GetAs (unsigned int index)
	{
		Assert(loaded_vector != 0, "Attempting to access NULL vector");
		return reinterpret_cast<GETTYPE>((*loaded_vector)[index]);
	}

	unsigned int Size()
	{
		Assert(loaded_vector != 0, "Attempting to access NULL vector");
		return loaded_vector->size();
	}
};

template<typename T>
void STLVectorObjectIO<T>::Save(Stream & stream) const
{
	if (stream.QueueForSerialization(this))
	{
		for (unsigned int i = 0; i < target_vector->size(); ++i)
		{
			const Serializable * item = static_cast<const Serializable *>((*target_vector)[i]);
			item->Save(stream);
		}
	}
}

template<typename T>
void STLVectorObjectIO<T>::Serialize(Stream & stream) const
{
	std::string name = "STLVectorObjectIO_";
	name.append(value_type_name);

	DataIO::WriteString(stream, name.c_str());
	DataIO::WriteInt(stream, target_vector->size());
	for (unsigned int i = 0; i < target_vector->size(); ++i)
	{
		const Serializable * item = static_cast<const Serializable *>((*target_vector)[i]);
		DataIO::WriteInt(stream, stream.GetId(item));
	}

	delete this;
}
template<typename T>
void STLVectorObjectIO<T>::Unserialize(Stream & stream)
{
	int target_size = DataIO::ReadInt(stream);
	loaded_vector->reserve(target_size);

	for (int i = 0; i < target_size; ++i)
	{
		loaded_vector->push_back((pointer) DataIO::ReadInt(stream));
	}
	
}
template<typename T>
void STLVectorObjectIO<T>::FixUp(Stream & stream)
{
	for (unsigned int i = 0; i < loaded_vector->size(); ++i)
	{
		(*loaded_vector)[i] = (pointer) stream.GetObjectForFixUp((unsigned int)(*loaded_vector)[i]);
	}
}

#undef TOSTRING

#endif