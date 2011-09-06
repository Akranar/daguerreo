#include "StackAllocator.h"

#include <iostream>

//#define _STACK_ALLOCATOR_DEBUG_OUT_

StackAllocator::StackAllocator()
:
memory(0),
top(0),
end(0),
delete_memory(true)
{

}
StackAllocator::StackAllocator(unsigned int total_size)
:
memory(::operator new (total_size)),
top(memory),
end(((char *) top) + total_size),
delete_memory(true)
{

#ifdef _STACK_ALLOCATOR_DEBUG_OUT_
	std::cout << "Top: " << top << std::endl;
	std::cout << "End: " << end << std::endl;
	std::cout << "Size: " << GetFreeSize() << std::endl;
#endif

}
StackAllocator::~StackAllocator()
{
	if (delete_memory && memory)
	{
		::delete memory;
	}
}

void * StackAllocator::Allocate(unsigned int size)
{
	//Note: Return null if we run out of memory.
	if (size > GetFreeSize())
	{
		return 0;
	}
	//Note: Return top and increment top by "size".
	void * rtn = top;
	top = ((char *) top) + size;

#ifdef _STACK_ALLOCATOR_DEBUG_OUT_
	std::cout << "Allocate Address: " << rtn << std::endl;
#endif

	return rtn;
}
void StackAllocator::Deallocate(void * pointer)
{

	//Note: If pointer > top, we're freeing memory from below the top of stack.
	//	-> Interpret as an out of order deallocation.
	if (pointer > top)
	{
#ifdef _DEBUG
		std::cout << "FATAL ERROR: Out of order deallocation detected in StackAllocator." << std::endl;
		throw 1;
#else
		return;
#endif
	}


#ifdef _STACK_ALLOCATOR_DEBUG_OUT_
	std::cout << "Deallocate Address: " << pointer << std::endl;
#endif

	top = pointer;
}