#ifndef _SERIALIZATION_LIB_H_
#define _SERIALIZATION_LIB_H_

/*
	Two-Pass Serialization/Unserialization Algorithms
		Serialization:
			-> Pass One:
				-> Call Serializable::Save(Stream &) for all objects.
					-> Serializable::Save(Stream &) calls Stream::QueueForSerialization().
			-> Pass Two:
				-> Call Stream::SerializeQueue().
					-> Calls Serializable::Serialize(Stream &).
		Unserialization:
			-> Pass One:
				-> Call LoadObjects(const ObjectFactory<Serializable> & factory)
					-> Reads name of object type.
						-> Passes it to factory to create object.
							-> Uses object's default constructor. (Must be defined)
					-> Calls Serializable::Unserialize(Stream &)
						-> Sets state for object based on values read from file.

	Requirements for Serialization
		-> Must have default constructor taking zero arguments.
		-> No invalid pointers can occur.
		-> Object members (actual objects, not pointers) cannot be pointed at by other objects.
			-> Undefined behavior after loading otherwise.
		-> Must Inherit and Implement Serializable Interface.
			-> In Save()
				-> Call Stream.QueueForSerialization(*this)
					-> Only if True:
						-> Call Save() in children.
			-> In Serialize()
				-> If children are not null:
					-> Get Integer ID from stream of children.
					-> Write ID to child.
			-> In Unserialize()
				-> Read in state data from stream.
				-> For IDs, cast and place in Pointer members for now.
			-> In FixUp()
				-> Convert IDs to Pointer addresses.
					-> Null pointer addresses automatically taken care of by Stream class.
*/


/*
	===================
		In Stream.h
	===================

	IMPORTANT:
		Argument Coercion
			-> C++ coerces pointers into bool
				-> Allows expressions like:
					-> if (pointer) { ... }
				-> Causes trouble here
					-> Can cause wrong function to be called when pointer passed.
					-> Fix for Stream::Save
						-> Pointers passed as "const TYPE *"
						-> bools passed as "const bool &"
					-> Fix for Stream::Load
						-> In DECLARE_LOAD_TEMPLATES
							-> template <typename T> bool Load(T *& value)
								-> Matches with pointers.
							-> While Stream::Load(bool &) matches with bools.
						-> UPDATE:
							-> Seems to not coerce with Load because coercion only happens when:
								-> arguments are const.
								
	Experiment Results:
		Pointer to Bool Conversions:
			-> pointer = bool
			-> const pointer = bool
			-> pointer = const bool reference
			-> const pointer = const bool reference

		Conversions That Don't Work:
			-> pointer  != bool reference
			-> const pointer != bool reference

		Conclusion:
			When a function expects a non_const reference, it won't do the conversions.

*/

#include "Serializable.h"
#include "Stream.h"
#include "STLSerializer.h"
#include "ArraySerializer.h"

#endif