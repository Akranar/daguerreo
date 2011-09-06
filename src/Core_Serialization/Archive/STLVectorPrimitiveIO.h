#ifndef _STL_VECTOR_IO_H_
#define _STL_VECTOR_IO_H_

#include <string>
#include <fstream>

#include <vector>

#include "DataIO.h"
#include "Stream.h"



class STLVectorPrimitiveIO
{
public:
	template<typename T>
	static void ReadSTLVector(Stream & stream, std::vector<T> & rtn_vector);

	template<typename T>
	static void ReadSTLVector(Stream & stream, std::vector<T *> & rtn_vector);

	template<typename T>
	static bool ReadSTLVector(Stream & stream, std::vector<const T *> & rtn_vector);

	template<typename T>
	static bool WriteSTLVector(Stream & stream, std::vector<T> & value);

	template<typename T>
	static bool WriteSTLVector(Stream & stream, std::vector<T *> & value);

	template<typename T>
	static bool WriteSTLVector(Stream & stream, std::vector<const T *> & value);

};

template<typename T>
void STLVectorPrimitiveIO::ReadSTLVector(Stream & stream, std::vector<T> & rtn_vector)
{
	int size = DataIO::ReadInt(stream);
}

template<typename T>
void STLVectorPrimitiveIO::ReadSTLVector(Stream & stream, std::vector<T *> & rtn_vector)
{
	int size = DataIO::ReadInt(stream);
}


template<typename T>
bool STLVectorPrimitiveIO::WriteSTLVector(Stream & stream, std::vector<T> & value)
{
	int size = value.size();
	DataIO::WriteInt(stream, size);
	for (unsigned int i = 0; i < size; ++i)
	{
		value[i].Serialize(stream);
	}
	return !stream.bad();
}

template<typename T>
bool STLVectorPrimitiveIO::WriteSTLVector(Stream & stream, std::vector<T *> & value)
{
	int size = value.size();
	DataIO::WriteInt(stream, size);
	for (unsigned int i = 0; i < size; ++i)
	{
		value[i]->Save(stream, false);
	}
	for (unsigned int i = 0; i < size; ++i)
	{
		DataIO::WriteInt(stream, stream.GetId(value[i]));
	}
	return !stream.bad();
}

template<typename T>
bool STLVectorPrimitiveIO::ReadSTLVector(Stream & stream, std::vector<const T *> & rtn_vector)
{
	int size = DataIO::ReadInt(stream);
	for (unsigned int i = 0; i < size; ++i)
	{
		rtn_vector.push_back((const T *) DataIO::ReadInt(stream));
	}

	return !stream.bad();
}

template<typename T>
bool STLVectorPrimitiveIO::WriteSTLVector(Stream & stream, std::vector<const T *> & value)
{
	int size = value.size();
	DataIO::WriteInt(stream, size);
	for (unsigned int i = 0; i < size; ++i)
	{
		value[i]->Save(stream, false);
	}
	for (unsigned int i = 0; i < size; ++i)
	{
		DataIO::WriteInt(stream, stream.GetId(value[i]));
	}
	return !stream.bad();
}


template<>
bool STLVectorPrimitiveIO::WriteSTLVector(Stream & stream, std::vector<int> & value);

template<>
void STLVectorPrimitiveIO::ReadSTLVector(Stream & stream, std::vector<int> & rtn_vector);

template<>
bool STLVectorPrimitiveIO::WriteSTLVector(Stream & stream, std::vector<float> & value);

template<>
void STLVectorPrimitiveIO::ReadSTLVector(Stream & stream, std::vector<float> & rtn_vector);





#endif