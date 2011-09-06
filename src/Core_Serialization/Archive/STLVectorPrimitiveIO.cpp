#include "STLVectorPrimitiveIO.h"

template<>
bool STLVectorPrimitiveIO::WriteSTLVector(Stream & stream, std::vector<int> & value)
{
	unsigned int size = value.size();
	DataIO::WriteInt(stream, size);
	for (unsigned int i = 0; i < size; ++i)
	{
		DataIO::WriteInt(stream, value[i]);
	}
	return stream.Status();
}


template<>
void STLVectorPrimitiveIO::ReadSTLVector(Stream & stream, std::vector<int> & rtn_vector)
{
	unsigned int size = DataIO::ReadInt(stream);
	for (unsigned int i = 0; i < size; ++i)
	{
		rtn_vector.push_back(DataIO::ReadInt(stream));
	}
}

template<>
bool STLVectorPrimitiveIO::WriteSTLVector(Stream & stream, std::vector<float> & value)
{
	unsigned int size = value.size();
	DataIO::WriteInt(stream, size);
	for (unsigned int i = 0; i < size; ++i)
	{
		DataIO::WriteFloat(stream, value[i]);
	}
	return stream.Status();
}

template<>
void STLVectorPrimitiveIO::ReadSTLVector(Stream & stream, std::vector<float> & rtn_vector)
{
	unsigned int size = DataIO::ReadInt(stream);
	for (unsigned int i = 0; i < size; ++i)
	{
		rtn_vector.push_back(DataIO::ReadFloat(stream));
	}
}