#ifndef _SERIALIZATION_TEST_H_
#define _SERIALIZATION_TEST_H_

#include "Serializable.h"

#include <string>
#include <iostream>

//Forward Declarations
class Stream;

//Tests
void TestObjectSerialization();
void TestObjectSerialization2();
void TestObjectSerialization3();

//Classes Used In Tests
class ObjectA : public Serializable
{
public:
	int data0;
	float data1;
	double data2;
	std::string data3;
	void Set(int data0, float data1, double data2, const char * data3)
	{
		this->data0 = data0;
		this->data1 = data1;
		this->data2 = data2;
		this->data3 = std::string(data3);
	}
	void Print() const;

	void Serialize(Stream & stream) const;
	void Unserialize(Stream & stream);
	void FixUp(Stream & stream);
	void Save(Stream & stream) const;
	void SerializeTypeId(Stream & stream) const;
};

class ObjectB;
class ObjectC;

class ObjectB : public Serializable
{
public:
	int data0;
	float data1;
	double data2;
	std::string data3;
	ObjectC * data4;

	void Set(int data0, float data1, double data2, const char * data3)
	{
		this->data0 = data0;
		this->data1 = data1;
		this->data2 = data2;
		this->data3 = std::string(data3);
		this->data4 = NULL; // MUST INITIALIZE ALL POINTERS.
	}
	void Print() const;

	void Serialize(Stream & stream) const;
	void Unserialize(Stream & stream);
	void FixUp(Stream & stream);
	void Save(Stream & stream) const;
	void SerializeTypeId(Stream & stream) const;
};

class ObjectC : public Serializable
{
public:
	int data0;
	float data1;
	double data2;
	std::string data3;
	ObjectB * data4;
	void Set(int data0, float data1, double data2, const char * data3)
	{
		this->data0 = data0;
		this->data1 = data1;
		this->data2 = data2;
		this->data3 = std::string(data3);
		this->data4 = NULL; // MUST INITIALIZE ALL POINTERS.
	}
	void Print() const;

	void Serialize(Stream & stream) const;
	void Unserialize(Stream & stream);
	void FixUp(Stream & stream);
	void Save(Stream & stream) const;
	void SerializeTypeId(Stream & stream) const;
};

class ObjectD : public Serializable
{
public:
	int data0;
	float data1;
	double data2;
	std::string data3;
	ObjectA object_a;
	void Set(int data0, float data1, double data2, const char * data3)
	{
		this->data0 = data0;
		this->data1 = data1;
		this->data2 = data2;
		this->data3 = std::string(data3);
	}
	ObjectA & AccessA()
	{
		return object_a;
	}
	void Print() const;

	void Serialize(Stream & stream) const;
	void Unserialize(Stream & stream);
	void FixUp(Stream & stream);
	void Save(Stream & stream) const;
	void SerializeTypeId(Stream & stream) const;
};

#endif