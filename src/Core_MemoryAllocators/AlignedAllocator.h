#ifndef _ALIGNED_ALLOCATOR_H_
#define _ALIGNED_ALLOCATOR_H_

#include "MemoryAllocator.h"

template <class INNER_ALLOCATOR_TYPE, int ALIGNMENT = 1>
class AlignedAllocator : public MemoryAllocator
{
	INNER_ALLOCATOR_TYPE * inner_allocator;
public:
	AlignedAllocator() : inner_allocator(0) {};
	AlignedAllocator(INNER_ALLOCATOR_TYPE * inner_allocator) : inner_allocator(inner_allocator) {};
	~AlignedAllocator() {};

	void * Allocate(unsigned int size);
	void Deallocate(void * pointer);
};

template <class INNER_ALLOCATOR_TYPE, int ALIGNMENT>
void * AlignedAllocator<INNER_ALLOCATOR_TYPE, ALIGNMENT>::Allocate(unsigned int size)
{
	unsigned int required_alignment = ALIGNMENT;
	unsigned int expanded_size = size + required_alignment;
	void * raw_address = inner_allocator->Allocate(expanded_size);
	//Note: "mask" holds the bytes that an aligned address must end with.
	//	-> To get the number of bytes we are off from an aligned address.
	//		-> Mask "raw_address" with "mask"	
	unsigned long mask = required_alignment - 1;
	unsigned int misalignment = (unsigned char) ((unsigned long)raw_address) & mask;
	unsigned char padding = required_alignment - misalignment;
	void * adjusted_rtn = ((char *) raw_address + padding);

	//Note: Padding will always be at least 1 if "required_alignment" is greater than 1.
	//	-> Store "padding" in the byte preceding "adjusted_rtn".
	*(((char *) adjusted_rtn)-1) = padding;

	return adjusted_rtn;
}

template <class INNER_ALLOCATOR_TYPE, int ALIGNMENT>
void AlignedAllocator<INNER_ALLOCATOR_TYPE, ALIGNMENT>::Deallocate(void * pointer)
{
	//Note: Retrieve the padding from the byte preceding pointer.
	//	-> Remove padding from pointer, then deallocate.
	void * aligned_address = pointer;
	unsigned char padding = *(((char *) aligned_address)-1);
	void * raw_address = ((char *) aligned_address) - padding;

	inner_allocator->Deallocate(raw_address);
}


template <class INNER_ALLOCATOR_TYPE, int ALIGNMENT>
inline void * operator new(size_t nbytes, AlignedAllocator<INNER_ALLOCATOR_TYPE, ALIGNMENT> & alloc)
{
	return alloc.Allocate(nbytes, ALIGNMENT);
}

template <class INNER_ALLOCATOR_TYPE, int ALIGNMENT>
inline void operator delete(void * pointer, AlignedAllocator<INNER_ALLOCATOR_TYPE, ALIGNMENT> & alloc)
{
	alloc.Deallocate(pointer);
}

#endif