#include "BinaryFileStream_Tests.h"
#include "BinaryFileStream.h"

#include <Core_ObjectFactory.h>
#include <Core_Assert.h>
#include <Core_FixedArray.h>

void BinaryFileStream_Tests::Entry()
{
	ObjectFactory<Serializable> factory;

	const char * out_file = "bfstests.txt";
	const char * in_file = "bfstests.txt";

	const char * some_text = "Some Text";
	int value0 = -1337;
	unsigned int value1 = 1337;
	short value2 = 5;
	unsigned short value3 = 60;
	char value4 = 42;
	unsigned char value5 = 69;
	long value6 = 42366;
	unsigned long value7 = 45600;
	float value8 = 1337.1337f;
	double value9 = 4000.6001;
	bool value10 = true;
	FixedArray<unsigned char> byte_array;
	byte_array.Set(10);
	for (unsigned int i = 0; i < byte_array.GetSize(); ++i)
	{
		byte_array[i] = i;
	}


	BinaryFileStream stream;
	stream.StartWrite(out_file);
	stream.Save(some_text);
	stream.Save(value0);
	stream.Save(value1);
	stream.Save(value2);
	stream.Save(value3);
	stream.Save(value4);
	stream.Save(value5);
	stream.Save(value6);
	stream.Save(value7);
	stream.Save(value8);
	stream.Save(value9);
	stream.Save(value10);
	STLSerializer<FixedArray<unsigned char> > array_serializer(stream);
	array_serializer.Save(byte_array);
	stream.Close();


	const char * read_some_text;
	int read_value0;
	unsigned int read_value1;
	short read_value2;
	unsigned short read_value3;
	char read_value4;
	unsigned char read_value5;
	long read_value6;
	unsigned long read_value7;
	float read_value8;
	double read_value9;
	bool read_value10;
	FixedArray<unsigned char> read_byte_array;

	stream.StartRead(in_file, factory);
	stream.Load(read_some_text);
	stream.Load(read_value0);
	stream.Load(read_value1);
	stream.Load(read_value2);
	stream.Load(read_value3);
	stream.Load(read_value4);
	stream.Load(read_value5);
	stream.Load(read_value6);
	stream.Load(read_value7);
	stream.Load(read_value8);
	stream.Load(read_value9);
	stream.Load(read_value10);
	array_serializer.Load(read_byte_array);
	stream.Close();

	Assert(strcmp(some_text, read_some_text) == 0, "Some Text Mismatch.");
	Assert(value0 == read_value0, "Value0 Mismatch.");
	Assert(value1 == read_value1, "Value1 Mismatch.");
	Assert(value2 == read_value2, "Value2 Mismatch.");
	Assert(value3 == read_value3, "Value3 Mismatch.");
	Assert(value4 == read_value4, "Value4 Mismatch.");
	Assert(value5 == read_value5, "Value5 Mismatch.");
	Assert(value6 == read_value6, "Value6 Mismatch.");
	Assert(value7 == read_value7, "Value7 Mismatch.");
	Assert(value8 == read_value8, "Value8 Mismatch.");
	Assert(value9 == read_value9, "Value9 Mismatch.");
	Assert(value10 == read_value10, "Value10 Mismatch.");
	for (unsigned int i = 0; i < byte_array.GetSize(); ++i)
	{
		Assert(byte_array[i] == read_byte_array[i], "Array element mismatch.");
	}

	delete read_some_text;

	std::cout << "BinaryFileStream Tests Successful!" << std::endl;
	getchar();
}