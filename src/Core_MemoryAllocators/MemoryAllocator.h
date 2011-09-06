#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

#include <cstddef>

class MemoryAllocator
{
public:
	virtual ~MemoryAllocator() {};
	virtual void * Allocate(unsigned int size) = 0;
	virtual void Deallocate(void * pointer) = 0;
};

inline void * operator new(size_t nbytes, MemoryAllocator & alloc)
{
	return alloc.Allocate(nbytes);
}

inline void operator delete(void * pointer, MemoryAllocator & alloc)
{
	alloc.Deallocate(pointer);
}

#endif