#ifndef _BINARY_FILE_STREAM_H_
#define _BINARY_FILE_STREAM_H_

#include "ExLib_Serialization.h"
#include <Core_EndianAbstractor.h>
#include <fstream>

class Serializable;

class BinaryFileStream : public Stream
{
private:
	std::fstream stream;
	EndianAbstractor endian;
	
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

	//**************************************************************************************
	//	Note:
	//		-> Declares template functions
	//			-> For the Load interface of Serializable type.
	//		-> Since template functions cannot be inherited,
	//			-> Use this macro to declare these methods in every subclass.
	//**************************************************************************************
	DECLARE_LOAD_TEMPLATED;

	//14 Basic Types
	bool Save(const char * val, int length = 1); //C-Style Strings
	bool Save(const Serializable & val);
	bool Save(const unsigned int & val);
	bool Save(const unsigned char & val);
	bool Save(const unsigned short & val);
	bool Save(const unsigned long & val);
	bool Save(const int & val);
	bool Save(const float & val);
	bool Save(const double & val);
	bool Save(const char & val);
	bool Save(const short & val);
	bool Save(const long & val);
	bool Save(const long double & val);
	bool Save(const bool & val);
	
	//13 Array Types
	bool Save(const char ** val, int length = 1); //Array of C-Style Strings
	bool Save(const Serializable * val); //Object Pointer/Array
	bool Save(const unsigned int * val, int length = 1);
	bool Save(const unsigned char * val, int length = 1);
	bool Save(const unsigned short * val, int length = 1);
	bool Save(const unsigned long * val, int length = 1);
	bool Save(const int * val, int length = 1);
	bool Save(const float * val, int length = 1);
	bool Save(const double * val, int length = 1);
	bool Save(const short * val, int length = 1);
	bool Save(const long * val, int length = 1);
	bool Save(const long double * val, int length = 1);
	bool Save(const bool * val, int length = 1);

	//13 2-Dimensional Array Types
	bool Save(const char *** val, int length = 2, int * lengths = NULL); //2D-Array of C-Style Strings
	bool Save(const Serializable ** val, int length = 2, int * lengths = NULL);
	bool Save(const unsigned int ** val, int level = 2, int * lengths = NULL);
	bool Save(const unsigned char ** val, int level = 2, int * lengths = NULL);
	bool Save(const unsigned short ** val, int level = 2, int * lengths = NULL);
	bool Save(const unsigned long ** val, int level = 2, int * lengths = NULL);
	bool Save(const int ** val, int level = 2, int * lengths = NULL);
	bool Save(const float ** val, int length = 2, int * lengths = NULL);
	bool Save(const double ** val, int length = 2, int * lengths = NULL);
	bool Save(const short ** val, int length = 2, int * lengths = NULL);
	bool Save(const long ** val, int length = 2, int * lengths = NULL);
	bool Save(const long double ** val, int length = 2, int * lengths = NULL);
	bool Save(const bool ** val, int length = 2, int * lengths = NULL);

	//14 Basic Types
	bool Load(char *& val, int length = 1);
	bool Load(Serializable & val);
	bool Load(unsigned int & val);
	bool Load(unsigned char & val);
	bool Load(unsigned short & val);
	bool Load(unsigned long & val);
	bool Load(int & val);
	bool Load(float & val);
	bool Load(double & val);
	bool Load(char & val);
	bool Load(short & val);
	bool Load(long & val);
	bool Load(long double & val);
	bool Load(bool & val);
	
	//13 Array Types
	bool Load(const char **& val, int length = 1); //2D-Array of C-Style Strings
	bool Load(Serializable *& val);
	bool Load(unsigned int *& val, int length = 1);
	bool Load(unsigned short *& val, int length = 1);
	bool Load(unsigned long *& val, int length = 1);
	bool Load(unsigned char *& val, int length = 1);
	bool Load(int *& val, int length = 1);
	bool Load(float *& val, int length = 1);
	bool Load(double *& val, int length = 1);
	bool Load(short *& val, int length = 1);
	bool Load(long *& val, int length = 1);
	bool Load(long double *& val, int length = 1);
	bool Load(bool *& val, int length = 1);


	//13 2D-Array Types
	bool Load(const char ***& val, int length = 2, int * lengths = NULL); //2D-Array of C-Style Strings
	bool Load(Serializable **& val, int length = 2, int * lengths = NULL);
	bool Load(unsigned int **& val, int length = 2, int * lengths = NULL);
	bool Load(unsigned short **& val, int length = 2, int * lengths = NULL);
	bool Load(unsigned long **& val, int length = 2, int * lengths = NULL);
	bool Load(int **& val, int level = 2, int * lengths = NULL);
	bool Load(float **& val, int length = 2, int * lengths = NULL);
	bool Load(double **& val, int length = 2, int * lengths = NULL);
	bool Load(char **& val, int length = 2, int * lengths = NULL);
	bool Load(short **& val, int length = 2, int * lengths = NULL);
	bool Load(long **& val, int length = 2, int * lengths = NULL);
	bool Load(long double **& val, int length = 2, int * lengths = NULL);
	bool Load(bool **& val, int length = 2, int * lengths = NULL);

	
	

};

#endif
