#ifndef _FIXED_ARRAY_H_
#define _FIXED_ARRAY_H_

//*************************************************************************************
//	Overview Notes:
//	-> FixedArray implemented differently if TYPE is object or if TYPE is primitive
//
//		-> Initialization with Constructors/Set with only size of array
//			-> Must initialize TYPE with default constructor if TYPE is an object.
//			-> Don't initialize if TYPE is built-in type primitive.
//
//		-> Copy Methods
//			-> Call Copy method of TYPE for objects
//			-> Copy by Assignment for primitives.
//
//		-> Destroy Methods
//			-> Don't need to call destructor for primitives
//				-> But they're called anyway here.
//
//		-> DestroyRecursive Methods only available for objects.
//
//*************************************************************************************

template <typename TYPE> struct FixedArray_IsFundamentalType { enum { result = false }; };

template <> struct FixedArray_IsFundamentalType<signed char> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<int> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<short> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<long> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<long long> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<unsigned char> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<unsigned int> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<unsigned short> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<unsigned long> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<unsigned long long> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<float> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<double> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<long double> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<bool> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<char> { enum { result = true }; };
template <> struct FixedArray_IsFundamentalType<wchar_t> { enum { result = true }; };
template <typename TYPE> struct FixedArray_IsFundamentalType<TYPE *> { enum { result = true }; };


template <typename TYPE, bool TYPE_IS_PRIMITIVE = FixedArray_IsFundamentalType<TYPE>::result, typename POINTER_TYPE = TYPE *>
class FixedArray
{
	unsigned int size;
	POINTER_TYPE data;
public:

	//Default Constructor
	inline FixedArray()
		: 
		size(0), 
		data(0)
	{
	}

	//Constructor with size TYPE to initialize array with.
	inline FixedArray(unsigned int size, const TYPE & init)
		: 
		size(size), 
		data(size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			new (data + i) TYPE();/////
			data[i] = init;
		}
	}

	//Copy constructor. Allocates memory with new.
	inline FixedArray(const FixedArray & copy)
		:
		size(copy.size), 
		data(size ? (TYPE *) ::operator new (copy.size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			new (data + i) TYPE();//////
			data[i] = copy.data[i];
		}
	}

	//Assignment operator. Allocates memory with new.
	inline void operator = (const FixedArray & assign)
	{
		size = assign.size;
		data = size ? (TYPE *) ::operator new (assign.size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			new (data + i) TYPE();/////
			data[i] = assign.data[i];
		}
	}

	template <class ALLOCATOR>
	inline FixedArray(const FixedArray & copy, ALLOCATOR & allocator)
		: 
		size(copy.size), 
		data(size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			new (data + i) TYPE();/////
			data[i] = copy.data[i];
		}
	}

	template <class ALLOCATOR>
	inline FixedArray(unsigned int size, const TYPE & init, ALLOCATOR & allocator)
		: 
		size(size), 
		data(size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			new (data + i) TYPE();//////
			data[i] = init;
		}
	}

	inline ~FixedArray()
	{
		Destroy();
	}

	template <class ALLOCATOR>
	inline void Set(unsigned int _size, const TYPE & init, ALLOCATOR & allocator)
	{
		if (data) Destroy(allocator);
		size = _size;
		data = size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			new (data + i) TYPE();//////
			data[i] = init;
		}
	}

	inline void Set(unsigned int _size, const TYPE & init)
	{
		if (data) Destroy();
		size = _size; 
		data = size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			new (data + i) TYPE();///////
			data[i] = init;
		}
	}

	inline const TYPE & operator[] (unsigned int index) const
	{
		return data[index];
	}
	inline TYPE & operator[] (unsigned int index)
	{
		return data[index];
	}

	inline unsigned int GetSize() const
	{
		return size;
	}

	inline POINTER_TYPE GetData() const
	{
		return data;
	}

	inline unsigned int GetOwnershipSize() const
	{
		return size * sizeof(TYPE);
	}

	//******************************************
	//	Destroy Methods
	//******************************************
	template <class ALLOCATOR>
	inline void DestroyRecursive(ALLOCATOR & allocator)
	{
		//Call destroy method on content.
		for (unsigned int i = GetSize(); i > 0; --i)
		{
			data[i-1].Destroy(allocator);
		}
		//Call destructors explicitly. Then deallocate memory.
		for (unsigned int i = size; i > 0; --i)
		{
			data[i-1].~TYPE();
		}
		allocator.Deallocate(data);
		size = 0;
		data = 0;
	}

	template <class ALLOCATOR>
	inline void Destroy(ALLOCATOR & allocator)
	{
		//Call destructors explicitly. Then deallocate memory.
		for (unsigned int i = size; i > 0; --i)
		{
			data[i-1].~TYPE();
		}
		allocator.Deallocate(data);
		size = 0;
		data = 0;
	}

	inline void DestroyRecursive()
	{
		//Call destroy method on content.
		for (unsigned int i = GetSize(); i > 0; --i)
		{
			data[i-1].Destroy();
		}
		//Call destructors explicitly. Then deallocate memory.
		for (unsigned int i = size; i > 0; --i)
		{
			data[i-1].~TYPE();
		}
		delete (void *) data;
		size = 0;
		data = 0;
	}

	inline void Destroy()
	{
		//Call destructors explicitly. Then deallocate memory.
		for (unsigned int i = size; i > 0; --i)
		{
			data[i-1].~TYPE();
		}
		delete (void *) data;
		size = 0;
		data = 0;
	}

	//*****************************************************************
	//Note: 
	//	-> The following methods require a default constructor for TYPE
	//*****************************************************************
	inline FixedArray(unsigned int size)
		: 
		size(size), 
		data(size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			//data[i] = TYPE();
			new (data + i) TYPE();
		}
	}

	template <class ALLOCATOR>
	inline FixedArray(unsigned int size, ALLOCATOR & allocator)
		: 
		size(size), 
		data(size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			//data[i] = TYPE();
			new (data + i) TYPE();
		}
	}

	inline void Set(unsigned int _size)
	{
		if (data) Destroy();
		size = _size; 
		data = size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			//data[i]();// = TYPE();
			new (data + i) TYPE();
		}
	}

	template <class ALLOCATOR>
	inline void Set(unsigned int _size, ALLOCATOR & allocator)
	{
		if (data) Destroy(allocator);
		size = _size;
		data = size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = TYPE();
		}
	}

	//*****************************************************************
	//Note: 
	//	-> The following methods require Copy to be defined for TYPE
	//*****************************************************************
	template <class ALLOCATOR>
	inline void Copy(const FixedArray & copy, ALLOCATOR & allocator)
	{
		if (data) Destroy(allocator);
		size = copy.size;
		data = size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			//Note: construct using default constructor, then call Copy.
			data[i] = TYPE();
			data[i].Copy(copy.data[i], allocator);
		}
	}

	inline void Copy(const FixedArray & copy)
	{
		if (data) Destroy();
		size = copy.size;
		data = size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			//Note: construct using default constructor, then call Copy.
			data[i] = TYPE();
			data[i].Copy(copy.data[i]);
		}
	}
	
};

//************************************************************
//	Template Specialization for Built-in Types or Primitives
//************************************************************

template <typename TYPE, typename POINTER_TYPE>
class FixedArray<TYPE, true, POINTER_TYPE>
{
	unsigned int size;
	POINTER_TYPE data;
public:

	//Default Constructor
	inline FixedArray()
		: 
		size(0), 
		data(0)
	{
	}

	//Constructor with size TYPE to initialize array with.
	inline FixedArray(unsigned int size, const TYPE & init)
		: 
		size(size), 
		data(size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = init;
		}
	}

	//Copy constructor. Allocates memory with new.
	inline FixedArray(const FixedArray & copy)
		:
		size(copy.size), 
		data(size ? (TYPE *) ::operator new (copy.size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = copy.data[i];
		}
	}

	//Assignment operator. Allocates memory with new.
	inline void operator = (const FixedArray & assign)
	{
		size = assign.size;
		data = size ? (TYPE *) ::operator new (assign.size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = assign.data[i];
		}
	}

	template <class ALLOCATOR>
	inline FixedArray(const FixedArray & copy, ALLOCATOR & allocator)
		: 
		size(copy.size), 
		data(size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = copy.data[i];
		}
	}

	template <class ALLOCATOR>
	inline FixedArray(unsigned int size, const TYPE & init, ALLOCATOR & allocator)
		: 
		size(size), 
		data(size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0)
	{
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = init;
		}
	}

	inline ~FixedArray()
	{
		Destroy();
	}

	template <class ALLOCATOR>
	inline void Set(unsigned int _size, const TYPE & init, ALLOCATOR & allocator)
	{
		if (data) Destroy(allocator);
		size = _size;
		data = size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = init;
		}
	}

	inline void Set(unsigned int _size, const TYPE & init)
	{
		if (data) Destroy();
		size = _size; 
		data = size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = init;
		}
	}

	inline const TYPE & operator[] (unsigned int index) const
	{
		return data[index];
	}
	inline TYPE & operator[] (unsigned int index)
	{
		return data[index];
	}

	inline unsigned int GetSize() const
	{
		return size;
	}

	inline POINTER_TYPE GetData() const
	{
		return data;
	}

	inline unsigned int GetOwnershipSize() const
	{
		return size * sizeof(TYPE);
	}

	//*************************************
	//	Destroy methods
	//*************************************
	template <class ALLOCATOR>
	inline void Destroy(ALLOCATOR & allocator)
	{
		//Note: Destructors are called even though these are primitives to be safe
		//	-> Will be skipped by compiler (by C++ standard) in the case of primitives.
		
		for (unsigned int i = size; i > 0; --i)
		{
			data[i-1].~TYPE();
		}
		
		allocator.Deallocate(data);
		size = 0;
		data = 0;
	}

	inline void Destroy()
	{
		//Note: Destructors are called even though these are primitives to be safe
		//	-> Will be skipped by compiler (by C++ standard) in the case of primitives.
		for (unsigned int i = size; i > 0; --i)
		{
			data[i-1].~TYPE();
		}
		delete (void *) data;
		size = 0;
		data = 0;
	}

	//*****************************************************************
	//Note: The following methods are only for TYPE = built-in types.
	//			-> No constructors called for TYPE
	//*****************************************************************
	inline FixedArray(unsigned int size)
		: 
		size(size), 
		data(size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0)
	{
	}

	template <class ALLOCATOR>
	inline FixedArray(unsigned int size, ALLOCATOR & allocator)
		: 
		size(size), 
		data(size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0)
	{
	}

	inline void Set(unsigned int _size)
	{
		if (data) Destroy();
		size = _size; 
		data = size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0;
	}

	template <class ALLOCATOR>
	inline void Set(unsigned int _size, ALLOCATOR & allocator)
	{
		if (data) Destroy(allocator);
		size = _size;
		data = size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			data[i] = TYPE();
		}
	}

	//********************************
	//	Copy methods
	//********************************
	template <class ALLOCATOR>
	inline void Copy(const FixedArray & copy, ALLOCATOR & allocator)
	{
		if (data) Destroy(allocator);
		size = copy.size;
		data = size ? (TYPE *) allocator.Allocate(size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			//Note: Copy by assignment. (build-in types)
			data[i] = copy.data[i]; 
		}
	}
	
	inline void Copy(const FixedArray & copy)
	{
		if (data) Destroy();
		size = copy.size;
		data = size ? (TYPE *) ::operator new (size * sizeof(TYPE)) : 0;
		for (unsigned int i = 0; i < size; ++i)
		{
			//Note: Copy by assignment. (build-in types)
			data[i] = copy.data[i]; 
		}
	}
};

#endif