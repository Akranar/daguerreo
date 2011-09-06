#include "BinaryFileStream.h"
#include "ExLib_Serialization.h"


BinaryFileStream::BinaryFileStream()
: endian(true)
{
	
}

BinaryFileStream::~BinaryFileStream()
{
	
}

void BinaryFileStream::StartRead(const char * filename, const ObjectFactory<Serializable> & factory)
{
	if (stream.is_open()) throw 1;
	stream.open(filename, std::ios_base::in|std::ios_base::binary);
	load_factory = &factory;
}

void BinaryFileStream::StartWrite(const char * filename)
{
	if (stream.is_open()) throw 1;
	stream.open(filename, std::ios_base::out|std::ios_base::binary);
}

void BinaryFileStream::Close()
{
	Stream::Close();
	stream.close();
}

char BinaryFileStream::Get()
{
	return stream.get();
}
void BinaryFileStream::Put(char value)
{
	stream.put(value);
}

bool BinaryFileStream::Status()
{
	return stream.good();
}