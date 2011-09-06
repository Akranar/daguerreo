#include "TEST_Serialization.h"
#include "Serialization_LIB.h"
#include "ExLib_Asserts.h"
#include "ExLib_ObjectCreation.h"
#include "TEST_SerializationClasses.h"

void TEST_Serialization::SimpleSaveLoad()
{

	ObjectFactory<Serializable> factory;
	factory.Register("ObjectA", new ObjectCreator<ObjectA, Serializable>());

	
	ObjectA * save_pointer = new ObjectA(3, 4, 5);
	int save_int = 5;
	float save_float = 3.5f;
	double save_double = 3.5;
	char save_char = '5';
	bool save_bool = true;
	short save_short = 7;
	long save_long = 67;
	long double save_longdouble = 254.344;
	const char * save_string = "wee";
	unsigned int save_uint = 34;
	unsigned char save_uchar = '3';
	unsigned short save_ushort = 232;
	unsigned long save_ulong = 34 ;
	ObjectA save_object = ObjectA(6, 7, 8);

	ObjectA * load_pointer;
	int load_int = 0;
	float load_float = 0;
	double load_double = 0;
	char load_char = 0;
	bool load_bool = 0;
	short load_short = 0;
	long load_long = 0;
	long double load_longdouble = 0;
	char * load_string = 0;
	unsigned int load_uint = 0;
	unsigned char load_uchar = 0;
	unsigned short load_ushort = 0;
	unsigned long load_ulong = 0;
	ObjectA load_object;
	

	Stream  * file = new BinaryFileStream();
	file->StartWrite("test6.txt");

	file->Save(save_pointer);
	file->Save(save_int);
	file->Save(save_float);
	file->Save(save_double);
	file->Save(save_char);
	file->Save(save_bool);
	file->Save(save_short);
	file->Save(save_long);
	file->Save(save_longdouble);
	file->Save(save_string);
	file->Save(save_uint);
	file->Save(save_uchar);
	file->Save(save_ushort);
	file->Save(save_ulong);
	file->Save(save_object);

	file->Close();

	file->StartRead("test6.txt", factory);
	
	file->Load(load_pointer);
	file->Load(load_int);
	file->Load(load_float);
	file->Load(load_double);
	file->Load(load_char);
	file->Load(load_bool);
	file->Load(load_short);
	file->Load(load_long);
	file->Load(load_longdouble);
	file->Load(load_string);
	file->Load(load_uint);
	file->Load(load_uchar);
	file->Load(load_ushort);
	file->Load(load_ulong);
	file->Load(load_object);

	file->Close();
	delete file;
	
	Assert(save_int == load_int, "int equal");
	Assert(save_float == load_float, "float not equal");
	Assert(save_double == load_double, "double not equal");
	Assert(save_char == load_char, "char not equal");
	Assert(save_bool == load_bool, "bool not equal");
	Assert(save_short == load_short, "short not equal");
	Assert(save_long == load_long, "long not equal");
	Assert(save_longdouble == load_longdouble, "long not equal");
	Assert(strcmp(save_string, load_string) == 0, "strings not equal");
	Assert(save_uint == load_uint, "uint not equal");
	Assert(save_uchar == load_uchar, "uchar not equal");
	Assert(save_ushort == load_ushort, "ushort not equal");
	Assert(save_ulong == load_ulong, "ulong not equal");

	delete save_pointer;
	delete load_pointer;
	delete load_string;
}


void TEST_Serialization::SimpleSaveLoad1D()
{
	ObjectFactory<Serializable> factory;
	factory.Register("ObjectA", new ObjectCreator<ObjectA, Serializable>());


	ObjectA * save_pointer = new ObjectA();
	save_pointer->Set(1, 2, 3);
	int * save_int = new int[3];
	float * save_float = new float[3];
	double * save_double = new double[3];
	char * save_char = new char[3];
	bool * save_bool = new bool[3];
	short * save_short = new short[3];
	long * save_long = new long[3];
	long double * save_longdouble = new long double[3];
	unsigned int * save_uint = new unsigned int[3];
	unsigned char * save_uchar = new unsigned char[3];
	unsigned short * save_ushort = new unsigned short[3];
	unsigned long * save_ulong = new unsigned long[3];
	

	ObjectA * load_pointer = 0;
	int * load_int = 0;
	float * load_float = 0;
	double * load_double = 0;
	char * load_char = 0;
	bool * load_bool = 0;
	short * load_short = 0;
	long * load_long = 0;
	long double * load_longdouble = 0;
	unsigned int * load_uint = 0;
	unsigned char * load_uchar = 0;
	unsigned short * load_ushort = 0;
	unsigned long * load_ulong = 0;
	

	Stream  * file = new BinaryFileStream();
	file->StartWrite("test7.txt");

	file->Save(save_pointer);
	file->Save(save_int, 3);
	file->Save(save_float, 3);
	file->Save(save_double, 3);
	file->Save(save_char, 3);
	file->Save(save_bool, 3);
	file->Save(save_short, 3);
	file->Save(save_long, 3);
	file->Save(save_longdouble, 3);
	file->Save(save_uint, 3);
	file->Save(save_uchar, 3);
	file->Save(save_ushort, 3);
	file->Save(save_ulong, 3);
	

	file->Close();

	file->StartRead("test7.txt", factory);
	
	file->Load(load_pointer);
	file->Load(load_int);
	file->Load(load_float);
	file->Load(load_double);
	file->Load(load_char);
	file->Load(load_bool);
	file->Load(load_short);
	file->Load(load_long);
	file->Load(load_longdouble);
	file->Load(load_uint);
	file->Load(load_uchar);
	file->Load(load_ushort);
	file->Load(load_ulong);
	


	file->Close();
	delete file;


	delete save_pointer;
	delete [] save_int;
	delete [] save_float;
	delete [] save_double;
	delete [] save_char;
	delete [] save_bool;
	delete [] save_short;
	delete [] save_long;
	delete [] save_longdouble;
	delete [] save_uint;
	delete [] save_uchar;
	delete [] save_ushort;
	delete [] save_ulong;
	

	delete load_pointer;
	delete [] load_int;
	delete [] load_float;
	delete [] load_double;
	delete [] load_char;
	delete [] load_bool;
	delete [] load_short;
	delete [] load_long;
	delete [] load_longdouble;
	delete [] load_uint;
	delete [] load_uchar;
	delete [] load_ushort;
	delete [] load_ulong;
	
}


void TEST_Serialization::Inheritance()
{
	ObjectFactory<Serializable> factory;
	factory.Register("ObjectB", new ObjectCreator<ObjectB, Serializable>());

	Stream * file = new BinaryFileStream();
	file->StartWrite("test8.txt");

	ObjectB * b = new ObjectB(1, 2, 3, 1337);

	*file << b;

	file->Close();

	file->StartRead("test8.txt", factory);
	
	ObjectB * load_b;
	file->Load(load_b);

	file->Close();
	delete file;
	delete b;
	delete load_b;
}

void TEST_Serialization::LeafJoins()
{
	ObjectFactory<Serializable> factory;
	factory.Register("ObjectA", new ObjectCreator<ObjectA, Serializable>());
	factory.Register("ObjectC", new ObjectCreator<ObjectC, Serializable>());

	Stream * file = new BinaryFileStream();
	file->StartWrite("test9.txt");

	ObjectA * a = new ObjectA(1, 2, 3);
	ObjectC * c = new ObjectC(4, 5, 6, a);
	ObjectC * c2 = new ObjectC(7, 8, 9, a);

	*file << c;
	*file << c2;
	*file << a;

	file->Close();

	file->StartRead("test9.txt", factory);
	
	ObjectC * load_c;
	ObjectC * load_c2;
	ObjectA * load_a;
	file->Load(load_c);
	file->Load(load_c2);
	file->Load(load_a);

	load_c->a->Print();
	load_c2->a->Print();

	load_a->Set(12, 12, 12);

	load_c->a->Print();
	load_c2->a->Print();


	file->Close();
	delete file;
	delete c;
	delete c2;
	delete a;
	delete load_c;
	delete load_c2;
	delete load_a;
}