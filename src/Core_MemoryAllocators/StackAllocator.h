#ifndef _STACK_ALLOCATOR_H_
#define _STACK_ALLOCATOR_H_

#include "MemoryAllocator.h"

class StackAllocator : public MemoryAllocator
{
	void * memory;
	void * top;
	void * end;
	bool delete_memory;
public:
	StackAllocator();
	StackAllocator(unsigned int total_size);
	StackAllocator(void * _memory_start, unsigned int total_size, bool owns_memory)
	:
		memory(_memory_start),
		top(memory),
		end(((char *) top) + total_size),
		delete_memory(owns_memory)
	{
	}
	~StackAllocator();

	void * Allocate(unsigned int size);
	void Deallocate(void * pointer);

	inline unsigned int GetFreeSize() const;
};

inline unsigned int StackAllocator::GetFreeSize() const
{
	return (unsigned int) ((char *) end - (char *) top);
}

inline void * operator new(size_t nbytes, StackAllocator & alloc)
{
	return alloc.Allocate(nbytes);
}

inline void operator delete(void * pointer, StackAllocator & alloc)
{
	alloc.Deallocate(pointer);
}

#endif