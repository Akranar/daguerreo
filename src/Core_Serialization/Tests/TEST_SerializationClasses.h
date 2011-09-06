#ifndef _TEST_SERIALIZATION_CLASSES_H_
#define _TEST_SERIALIZATION_CLASSES_H_

#include "Serializable.h"

class ObjectA : public Serializable
{
public:
	int data1;
	int data2;
	int data3;
public:
	ObjectA()
	{
		int x = 1;
	};
	ObjectA(int a, int b, int c) : data1(a), data2(b), data3(c) {};
	void Set(int a, int b, int c) 
	{
		data1 = a;
		data2 = b;
		data3 = c;
	}
	void Serialize(Stream & stream) const
	{
		stream.Save(data1);
		stream.Save(data2);
		stream.Save(data3);
	}
	void SerializeTypeId(Stream & stream) const
	{
		const char * id = "ObjectA";
		stream.Save(id);
	}
	void Unserialize(Stream & stream)
	{
		stream.Load(data1);
		stream.Load(data2);
		stream.Load(data3);
	}
	void Print()
	{
		std::cout << "A: " << data1 << ", " << data2 << ", " << data3 << std::endl;
	}
};

class ObjectB : public ObjectA
{
public:
	int data4;
public:
	ObjectB(){};
	ObjectB(int a, int b, int c, int d) : ObjectA(a, b, c), data4(d) {};
	void Set(int d) 
	{
		data4 = d;
	}
	void Serialize(Stream & stream) const
	{
		ObjectA::Serialize(stream);
		stream.Save(data4);
	}
	void SerializeTypeId(Stream & stream) const
	{
		const char * id = "ObjectB";
		stream.Save(id);
	}
	void Unserialize(Stream & stream)
	{
		ObjectA::Unserialize(stream);
		stream.Load(data4);
	}
	void Print()
	{
		std::cout << "B: " << data4 << std::endl;
	}
};

class ObjectC : public Serializable
{
public:
	int data1;
	float data2;
	double data3;
	ObjectA * a;
public:
	ObjectC() : a(NULL) {};
	ObjectC(int a, float b, double c) : data1(a), data2(b), data3(c) {};
	ObjectC(int d, float e, double f, ObjectA * a) : data1(d), data2(e), data3(f), a(a) {};
	void Point(ObjectA * a) 
	{
		this->a = a;
	}
	void Serialize(Stream & stream) const
	{
		stream.Save(data1);
		stream.Save(data2);
		stream.Save(data3);
		stream.Save(a);
	}
	void SerializeTypeId(Stream & stream) const
	{
		const char * id = "ObjectC";
		stream.Save(id);
	}
	void Unserialize(Stream & stream)
	{
		stream.Load(data1);
		stream.Load(data2);
		stream.Load(data3);
		stream.Load(a);
	}
};

#endif