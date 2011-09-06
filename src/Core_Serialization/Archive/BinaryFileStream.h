#ifndef _BINARY_FILE_STREAM_H_
#define _BINARY_FILE_STREAM_H_

#include "Stream.h"
#include <fstream>

class Serializable;

class BinaryFileStream : public Stream
{
private:
	std::fstream stream;
	
public:
	char Get();
	void Put(char value);
	BinaryFileStream();
	virtual ~BinaryFileStream();
	void StartRead(const char * filename, const ObjectFactory<Serializable> & factory);
	void StartWrite(const char * filename);
	void Close();
	bool Status();
	//bool EndOfFile();

	bool Save(const int & val);
	bool Save(const float & val);
	bool Save(const double & val);
	bool Save(const char & val);
	bool Save(const bool & val);
	bool Save(const short & val);
	bool Save(const long & val);
	bool Save(const Serializable & val);

	bool Load(int & val);
	bool Load(float & val);
	bool Load(double & val);
	bool Load(char & val);
	bool Load(bool & val);
	bool Load(short & val);
	bool Load(long & val);
	bool Load(Serializable & val);

	
	bool Save(const char *& val, int length = 1);
	bool Load(char *& val, int length = 1);

};

#endif