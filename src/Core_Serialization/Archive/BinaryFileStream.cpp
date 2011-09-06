#include "BinaryFileStream.h"
#include "Serializable.h"

BinaryFileStream::BinaryFileStream()
{
	
}

BinaryFileStream::~BinaryFileStream()
{
	
}

void BinaryFileStream::StartRead(const char * filename, const ObjectFactory<Serializable> & factory)
{
	if (stream.is_open()) throw 1;
	stream.open(filename, std::ios_base::in|std::ios_base::binary);
	this->LoadObjects(factory);
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




//===========

bool BinaryFileStream::Save(const int & val)
{
	DataIO::WriteInt(*this, val);
	return true;
}
bool BinaryFileStream::Save(const float & val)
{
	DataIO::WriteFloat(*this, val);
	return true;
}
bool BinaryFileStream::Save(const double & val)
{
	DataIO::WriteDouble(*this, val);
	return true;
}
bool BinaryFileStream::Save(const char & val)
{
	DataIO::WriteChar(*this, val);
	return true;
}
bool BinaryFileStream::Save(const bool & val)
{
	DataIO::WriteBool(*this, val);
	return true;
}
bool BinaryFileStream::Save(const short & val)
{
	DataIO::WriteShort(*this, val);
	return true;
}
bool BinaryFileStream::Save(const long & val)
{
	DataIO::WriteLong(*this, val);
	return true;
}



bool BinaryFileStream::Load(int & val)
{
	val = DataIO::ReadInt(*this);
	return true;
}
bool BinaryFileStream::Load(float & val)
{
	val = DataIO::ReadInt(*this);
	return true;
}
bool BinaryFileStream::Load(double & val)
{
	val = DataIO::ReadDouble(*this);
	return true;
}
bool BinaryFileStream::Load(char & val)
{
	val = DataIO::ReadChar(*this);
	return true;
}
bool BinaryFileStream::Load(bool & val)
{
	val = DataIO::ReadBool(*this);
	return true;
}
bool BinaryFileStream::Load(short & val)
{
	val = DataIO::ReadShort(*this);
	return true;
}
bool BinaryFileStream::Load(long & val)
{
	val = DataIO::ReadLong(*this);
	return true;
}


// Strings

bool BinaryFileStream::Save(const char *& val, int length)
{
	DataIO::WriteString(*this, val);
	return true;
}

bool BinaryFileStream::Load(char *& val, int length)
{
	val = DataIO::ReadString(*this);
	return true;
}


