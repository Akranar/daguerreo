#ifndef _STREAM_H_
#define _STREAM_H_


#include <vector>
#include <set>

#include "Serializable.h"
#include "StreamLoadTemplated.h"

#include <map>
#include <string>

template<typename Base>
class ObjectFactory;

class Stream
{
protected:

	const ObjectFactory<Serializable> * load_factory;
	std::set<const Serializable *> serialized_pointed_objects;
	std::map<const Serializable *, Serializable *> old_map_new;
public:
	virtual ~Stream() {};

	virtual void Close()
	{
		serialized_pointed_objects.clear();
		old_map_new.clear();
	}
	//virtual void Close(); //Subclasses that over write MUST call this in overridden method.

	//Abstract Methods
	virtual char Get() = 0;
	virtual void Put(char value) = 0;
	virtual void StartRead(const char * filename, const ObjectFactory<Serializable> & factory) = 0;
	virtual void StartWrite(const char * filename) = 0;
	virtual bool Status() = 0;

	bool EndOfFile();
	void SetEOF();


	//NEW
	Stream & operator << (const Serializable * val);
	Serializable * Next()
	{
		Serializable * rtn;
		Load(rtn);
		return rtn;
	}
	bool ShouldSerialize(const Serializable * val_address);
	bool ShouldUnserialize(const Serializable * old_address, Serializable *& rtn_new_address);
	void SetOldToNew(const Serializable * old_address, Serializable * new_address);

	//**************************************************************************************
	//	Note:
	//		-> Declares template functions
	//			-> For the Load interface of Serializable type.
	//		-> Since template functions cannot be inherited,
	//			-> Use this macro to declare these methods in every subclass.
	//**************************************************************************************
	DECLARE_LOAD_TEMPLATED;

	//14 Basic Types
	virtual bool Save(const char * val, int length = 1) = 0; //C-Style Strings
	virtual bool Save(const Serializable & val) = 0;
	virtual bool Save(const unsigned int & val) = 0;
	virtual bool Save(const unsigned char & val) = 0;
	virtual bool Save(const unsigned short & val) = 0;
	virtual bool Save(const unsigned long & val) = 0;
	virtual bool Save(const int & val) = 0;
	virtual bool Save(const float & val) = 0;
	virtual bool Save(const double & val) = 0;
	virtual bool Save(const char & val) = 0;
	virtual bool Save(const short & val) = 0;
	virtual bool Save(const long & val) = 0;
	virtual bool Save(const long double & val) = 0;
	virtual bool Save(const bool & val) = 0;
	
	//13 Array Types
	virtual bool Save(const char ** val, int length = 1) = 0; //Array of C-Style Strings
	virtual bool Save(const Serializable * val) = 0; //Object Pointer
	virtual bool Save(const unsigned int * val, int length = 1) = 0;
	virtual bool Save(const unsigned char * val, int length = 1) = 0;
	virtual bool Save(const unsigned short * val, int length = 1) = 0;
	virtual bool Save(const unsigned long * val, int length = 1) = 0;
	virtual bool Save(const int * val, int length = 1) = 0;
	virtual bool Save(const float * val, int length = 1) = 0;
	virtual bool Save(const double * val, int length = 1) = 0;
	virtual bool Save(const short * val, int length = 1) = 0;
	virtual bool Save(const long * val, int length = 1) = 0;
	virtual bool Save(const long double * val, int length = 1) = 0;
	virtual bool Save(const bool * val, int length = 1) = 0;

	//13 2-Dimensional Array Types
	virtual bool Save(const char *** val, int length = 2, int * lengths = NULL) = 0; //2D-Array of C-Style Strings
	virtual bool Save(const Serializable ** val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Save(const unsigned int ** val, int level = 2, int * lengths = NULL) = 0;
	virtual bool Save(const unsigned char ** val, int level = 2, int * lengths = NULL) = 0;
	virtual bool Save(const unsigned short ** val, int level = 2, int * lengths = NULL) = 0;
	virtual bool Save(const unsigned long ** val, int level = 2, int * lengths = NULL) = 0;
	virtual bool Save(const int ** val, int level = 2, int * lengths = NULL) = 0;
	virtual bool Save(const float ** val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Save(const double ** val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Save(const short ** val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Save(const long ** val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Save(const long double ** val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Save(const bool ** val, int length = 2, int * lengths = NULL) = 0;

	//14 Basic Types
	virtual bool Load(char *& val, int length = 1) = 0;
	virtual bool Load(Serializable & val) = 0;
	virtual bool Load(unsigned int & val) = 0;
	virtual bool Load(unsigned char & val) = 0;
	virtual bool Load(unsigned short & val) = 0;
	virtual bool Load(unsigned long & val) = 0;
	virtual bool Load(int & val) = 0;
	virtual bool Load(float & val) = 0;
	virtual bool Load(double & val) = 0;
	virtual bool Load(char & val) = 0;
	virtual bool Load(short & val) = 0;
	virtual bool Load(long & val) = 0;
	virtual bool Load(long double & val) = 0;
	virtual bool Load(bool & val) = 0;
	
	//13 Array Types
	virtual bool Load(const char **& val, int length = 1) = 0; //2D-Array of C-Style Strings
	virtual bool Load(Serializable *& val) = 0;
	virtual bool Load(unsigned int *& val, int length = 1) = 0;
	virtual bool Load(unsigned short *& val, int length = 1) = 0;
	virtual bool Load(unsigned long *& val, int length = 1) = 0;
	virtual bool Load(unsigned char *& val, int length = 1) = 0;
	virtual bool Load(int *& val, int length = 1) = 0;
	virtual bool Load(float *& val, int length = 1) = 0;
	virtual bool Load(double *& val, int length = 1) = 0;
	virtual bool Load(short *& val, int length = 1) = 0;
	virtual bool Load(long *& val, int length = 1) = 0;
	virtual bool Load(long double *& val, int length = 1) = 0;
	virtual bool Load(bool *& val, int length = 1) = 0;


	//13 2D-Array Types
	virtual bool Load(const char ***& val, int length = 2, int * lengths = NULL) = 0; //2D-Array of C-Style Strings
	virtual bool Load(Serializable **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(unsigned int **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(unsigned short **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(unsigned long **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(int **& val, int level = 2, int * lengths = NULL) = 0;
	virtual bool Load(float **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(double **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(char **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(short **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(long **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(long double **& val, int length = 2, int * lengths = NULL) = 0;
	virtual bool Load(bool **& val, int length = 2, int * lengths = NULL) = 0;
	
	
};

#endif