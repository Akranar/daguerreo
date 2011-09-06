#include "TEST_Serialization.h"
#include "ExLib_ObjectCreation.h"
#include "STLVectorObjectIO.h"
#include "BinaryFileStream.h"

void TestObjectSerialization()
{
	ObjectFactory<Serializable> factory;

	factory.Register("ObjectA", new ObjectCreator<ObjectA, Serializable>());
	factory.Register("ObjectB", new ObjectCreator<ObjectB, Serializable>());
	factory.Register("ObjectC", new ObjectCreator<ObjectC, Serializable>());
	factory.Register("STLVectorObjectIO", new ObjectCreator<STLVectorObjectIO<ObjectA>, Serializable>());


	ObjectA a = ObjectA();
	a.Set(1, 2, 3, "ObjectA baby");

	ObjectB b = ObjectB();
	b.Set(6, 7, 8, "ObjectB baby");
	

	ObjectC c = ObjectC();
	c.Set(3, 4, 5, "ObjectC baby");

	b.data4 = &c;
	c.data4 = &b;

	BinaryFileStream stream;

	stream.StartWrite("test.txt");
	a.Save(stream);
	b.Save(stream);
	//c.Save(stream, true);
	stream.SerializeQueue();
	stream.Close();

	std::vector<Serializable *> load;

	stream.StartRead("test.txt", factory);
	//stream.LoadObjects(factory);

	ObjectA * loaded_a = stream.GetNext<ObjectA*>();
	ObjectB * loaded_b = stream.GetNext<ObjectB*>();
	ObjectC * loaded_c = stream.GetNext<ObjectC*>();

	stream.Close();

	loaded_a->Print();
	loaded_b->Print();
	loaded_c->Print();

	delete loaded_a;
	delete loaded_b;
	delete loaded_c;
}

void TestObjectSerialization2()
{
	ObjectFactory<Serializable> factory;

	factory.Register("ObjectA", new ObjectCreator<ObjectA, Serializable>());
	factory.Register("ObjectB", new ObjectCreator<ObjectB, Serializable>());
	factory.Register("ObjectC", new ObjectCreator<ObjectC, Serializable>());
	factory.Register("STLVectorObjectIO_ObjectA", new ObjectCreator<STLVectorObjectIO<ObjectA>, Serializable>());
	factory.Register("STLVectorObjectIO_ObjectB", new ObjectCreator<STLVectorObjectIO<ObjectB>, Serializable>());
	factory.Register("STLVectorObjectIO_ObjectC", new ObjectCreator<STLVectorObjectIO<ObjectC>, Serializable>());
	factory.Register("STLVectorObjectIO_Serializable", new ObjectCreator<STLVectorObjectIO<Serializable>, Serializable>());


	
	ObjectC a = ObjectC();
	a.Set(1, 2, 3, "ObjectA baby");

	ObjectC b = ObjectC();
	b.Set(4, 5, 6, "ObjectB baby");

	ObjectC c = ObjectC();
	c.Set(7, 8, 9, "ObjectC baby");

	ObjectB d = ObjectB();
	d.Set(10, 11, 12, "ObjectD baby");

	ObjectB e = ObjectB();
	e.Set(13, 14, 15, "ObjectE baby");

	ObjectB f = ObjectB();
	f.Set(16, 17, 18, "ObjectF baby");

	a.data4 = &d;
	b.data4 = &e;
	c.data4 = &f;

	d.data4 = &a;
	e.data4 = &b;
	f.data4 = &c;


	BinaryFileStream stream;

	std::vector<ObjectC const *> to_serialize_C;
	to_serialize_C.push_back(&a);
	to_serialize_C.push_back(&b);
	to_serialize_C.push_back(&c);
	STLVectorObjectIO<ObjectC>  * proxy_C  = new STLVectorObjectIO<ObjectC>(&to_serialize_C, "ObjectC");

	std::vector<ObjectB const *> to_serialize_B;
	to_serialize_B.push_back(&d);
	to_serialize_B.push_back(&e);
	to_serialize_B.push_back(&f);
	STLVectorObjectIO<ObjectB>  * proxy_B  = new STLVectorObjectIO<ObjectB>(&to_serialize_B, "ObjectB");

	std::vector<Serializable *> to_serialize_master;
	to_serialize_master.push_back(proxy_C);
	to_serialize_master.push_back(proxy_B);
	STLVectorObjectIO<Serializable>  * proxy_master  = new STLVectorObjectIO<Serializable>(&to_serialize_master, "Serializable");


	stream.StartWrite("test2.txt");
	proxy_master->Save(stream);
	stream.SerializeQueue();
	stream.Close();
	

	std::vector<Serializable *> load;
	

	stream.StartRead("test2.txt", factory);
	STLVectorObjectIO<Serializable> * loaded_vector_proxy_M = stream.GetNext<STLVectorObjectIO<Serializable> *>();
	stream.SetEOF();


	STLVectorObjectIO<ObjectC> * loaded_vector_proxy_C = loaded_vector_proxy_M->GetAs<STLVectorObjectIO<ObjectC>*>(0);
	STLVectorObjectIO<ObjectB> * loaded_vector_proxy_B = loaded_vector_proxy_M->GetAs<STLVectorObjectIO<ObjectB>*>(1);
	
	for(unsigned int i = 0; i < loaded_vector_proxy_C->Size(); ++i)
	{
		(*loaded_vector_proxy_C)[i]->Print();
	}
	for(unsigned int i = 0; i < loaded_vector_proxy_B->Size(); ++i)
	{
		(*loaded_vector_proxy_B)[i]->Print();
	}

	loaded_vector_proxy_C->DeleteObjects();
	loaded_vector_proxy_B->DeleteObjects();
	loaded_vector_proxy_M->DeleteObjects();
	delete loaded_vector_proxy_M;
}



void TestObjectSerialization3()
{
	ObjectFactory<Serializable> factory;

	//factory.Register("ObjectA", new ObjectCreator<ObjectA, Serializable>());
	factory.Register("ObjectD", new ObjectCreator<ObjectD, Serializable>());


	ObjectD d = ObjectD();
	d.Set(6, 7, 8, "ObjectD baby");
	d.AccessA().Set(1, 2, 3, "ObjectA baby");
	
	BinaryFileStream stream;

	stream.StartWrite("test3.txt");
	d.Save(stream);
	stream.SerializeQueue();
	stream.Close();

	std::vector<Serializable *> load;

	stream.StartRead("test3.txt", factory);
	ObjectD * loaded = stream.GetNext<ObjectD*>();

	stream.Close();
	loaded->Print();
	delete loaded;
}