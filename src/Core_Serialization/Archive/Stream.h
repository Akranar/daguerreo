#ifndef _STREAM_H_
#define _STREAM_H_

#include <map>
#include <vector>
#include <set>

#include "Serializable.h"
#include "DataIO.h"
#include "ExLib_Containers.h"

#include "SerializationQueue.h"

template<typename Base>
class ObjectFactory;

class Stream
{
protected:
	/*
	typedef std::map<const Serializable *, unsigned int> OBJECT_ID_MAP;
	typedef std::vector<const Serializable *> OBJECT_VECTOR;
	typedef std::vector<Serializable *> LOADED_OBJECTS;
	typedef std::vector<std::pair<unsigned int, const Serializable *>> REMAPPING_QUEUE;

	OBJECT_ID_MAP object_id_map;
	OBJECT_VECTOR serialization_queue;
	LOADED_OBJECTS loaded_objects;
	REMAPPING_QUEUE remap_queue;

	
	void LoadObjects(const ObjectFactory<Serializable> & factory); // Starts Unseralize Pass
	//Iteration
	unsigned int current_index;
	*/
public:
	virtual ~Stream() {};
	virtual void Close(); //Subclasses that over write MUST call this in overridden method.
	const std::vector<Serializable *> & GetLoadedObjects() const;

	//Abstract Methods
	virtual char Get() = 0;
	virtual void Put(char value) = 0;
	virtual void StartRead(const char * filename, const ObjectFactory<Serializable> & factory) = 0;
	virtual void StartWrite(const char * filename) = 0;
	virtual bool Status() = 0;

	//Encapsulates Two-Pass Serialization/Unserialization Algorithms
	void SerializeQueue();// Starts Serialize Pass
	bool QueueForSerialization(const Serializable * object); //Save Pass
	void ReserveId(const Serializable & nested_object);  //Save Pass.
	void QueueRemapReservedId(unsigned int id, const Serializable & object); //Unserialize Pass
	int GetId(const Serializable * object) const; // Serialize + FixUp Passes
	Serializable * GetObjectForFixUp(unsigned int id) const; // FixUp Pass

	//Helper Functions to Serialize Nested Objects
	void NestedObject_Save(const Serializable & nested_object);
	void NestedObject_Serialize(const Serializable & nested_object);
	void NestedObject_Unserialize(Serializable & nested_object, const char * type_name);
	void NestedObject_FixUp(Serializable & nested_object);

	//Helper Functions to Serialize Pointers
	void Pointer_Save(const Serializable * pointer_object);
	void Pointer_Serialize(const Serializable * pointer_object);
	template<typename POINTER>
	POINTER Pointer_Unserialize(Serializable * pointer_object);
	template<typename POINTER>
	POINTER Pointer_FixUp(POINTER nested_object);

	//Read Objects
	template<typename U>
	U GetNext();
	Serializable * GetNext();
	bool EndOfFile();
	void SetEOF();



	//NEW
	const ObjectFactory<Serializable> * load_factory;
	std::set<const Serializable *> serialized_pointed_objects;
	std::map<const Serializable *, Serializable *> old_map_new;

	Stream & operator << (const Serializable * val)
	{
		Save(val);
		return *this;
	}

	Stream & operator >> (Serializable *& val)
	{
		Load(val);
		return *this;
	}

	Serializable * Load()
	{
		Serializable * rtn;
		this->Load(rtn);
		return rtn;
	}


	bool ShouldSerialize(const Serializable * val_address)
	{
		std::pair<std::set<const Serializable *>::iterator, bool> result = serialized_pointed_objects.insert(val_address);
		return result.second;
	}

	bool ShouldUnserialize(const Serializable * old_address, Serializable *& rtn_new_address)
	{
		std::map<const Serializable *, Serializable *>::iterator result = old_map_new.find(old_address);
		if (result != old_map_new.end())
		{
			rtn_new_address = result->second;
			return false;
		}
		else
		{
			return true;
		}
	}

	virtual bool Save(const int & val) = 0;
	virtual bool Save(const float & val) = 0;
	virtual bool Save(const double & val) = 0;
	virtual bool Save(const char & val) = 0;
	virtual bool Save(const bool & val) = 0;
	virtual bool Save(const short & val) = 0;
	virtual bool Save(const long & val) = 0;
	virtual bool Save(const char *& val, int length = 1) = 0;
	bool Save(const Serializable & val);

	bool Save(const int *& val, int length = 1);
	bool Save(const float *& val, int length = 1);
	bool Save(const double *& val, int length = 1);
	bool Save(const bool *& val, int length = 1);
	bool Save(const short *& val, int length = 1);
	bool Save(const long *& val, int length = 1);
	bool Save(const Serializable *& val, int length = 1);

	bool Save(const int **& val, int level = 2, int * lengths = NULL);
	bool Save(const float **& val, int length = 2, int * lengths = NULL);
	bool Save(const double **& val, int length = 2, int * lengths = NULL);
	bool Save(const char **& val, int length = 2, int * lengths = NULL);
	bool Save(const bool **& val, int length = 2, int * lengths = NULL);
	bool Save(const short **& val, int length = 2, int * lengths = NULL);
	bool Save(const long **& val, int length = 2, int * lengths = NULL);
	bool Save(const Serializable **& val, int length = 2, int * lengths = NULL);

	virtual bool Load(int & val) = 0;
	virtual bool Load(float & val) = 0;
	virtual bool Load(double & val) = 0;
	virtual bool Load(char & val) = 0;
	virtual bool Load(bool & val) = 0;
	virtual bool Load(short & val) = 0;
	virtual bool Load(long & val) = 0;
	virtual bool Load(char *& val, int length = 1) = 0;
	bool Load(Serializable & val);

	bool Load(int *& val, int length = 1);
	bool Load(float *& val, int length = 1);
	bool Load(double *& val, int length = 1);
	bool Load(bool *& val, int length = 1);
	bool Load(short *& val, int length = 1);
	bool Load(long *& val, int length = 1);
	bool Load(Serializable *& val, int length = 1);

	bool Load(int **& val, int level = 2, int * lengths = NULL);
	bool Load(float **& val, int length = 2, int * lengths = NULL);
	bool Load(double **& val, int length = 2, int * lengths = NULL);
	bool Load(char **& val, int length = 2, int * lengths = NULL);
	bool Load(bool **& val, int length = 2, int * lengths = NULL);
	bool Load(short **& val, int length = 2, int * lengths = NULL);
	bool Load(long **& val, int length = 2, int * lengths = NULL);
	bool Load(Serializable **& val, int length = 2, int * lengths = NULL);
};

template<typename POINTER>
POINTER Stream::Pointer_Unserialize(Serializable * pointer_object)
{
	return (POINTER) DataIO::ReadInt(*this);
}

template<typename POINTER>
POINTER Stream::Pointer_FixUp(POINTER nested_object)
{
	return (POINTER) GetObjectForFixUp((unsigned int)nested_object);
}

template<typename U>
U Stream::GetNext()
{
	U rtn = reinterpret_cast<U>(loaded_objects[current_index]);
	current_index += 1;
	return rtn;
}

#endif