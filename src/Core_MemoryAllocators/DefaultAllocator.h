#ifndef _DEFAULT_ALLOCATOR_H_
#define _DEFAULT_ALLOCATOR_H_

#include <new>

class DefaultAllocator
{
public:
	inline void * Allocate(unsigned int size);
	inline void Deallocate(void * pointer);
};


inline void * DefaultAllocator::Allocate(unsigned int size)
{
	return ::operator new(size);
}
inline void DefaultAllocator::Deallocate(void * pointer)
{
	::delete pointer;
}

inline void * operator new(size_t nbytes, DefaultAllocator & alloc)
{
	return alloc.Allocate(nbytes);
}

inline void operator delete(void * pointer, DefaultAllocator & alloc)
{
	alloc.Deallocate(pointer);
}


//placement new array unusable!!!

#endif