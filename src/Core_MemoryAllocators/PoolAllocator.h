#ifndef _POOL_ALLOCATOR_H_
#define _POOL_ALLOCATOR_H_

#include <vector>
#include "MemoryAllocator.h"
#include <Core_AutoPointer.h>

#define MEMORY_ADDRESS void *

class PoolAllocator : public MemoryAllocator
{
private:
	std::vector<MEMORY_ADDRESS *> pages;
	size_t page_size;
	unsigned int memaddr_count_per_block;
	unsigned int blocks_available;
	MEMORY_ADDRESS * free_list;
	unsigned int blocks_per_page;
public:
	PoolAllocator(size_t page_size, size_t allocation_size);
	~PoolAllocator();
	void * Allocate(unsigned int size);
	void Deallocate(void * pointer);
	void FreeAll();
	inline unsigned int FreeCount() const;
	inline unsigned int PageSize() const;
	inline unsigned int ObjectSize() const;
	inline unsigned int MaxCount() const;
	inline unsigned int UsedCount() const;
	inline unsigned int BlocksPerPage() const;
private:
	void ConfigurePage(MEMORY_ADDRESS * page);
	void FetchPage();
};

inline unsigned int PoolAllocator::FreeCount() const
{
	return blocks_available;
}

inline unsigned int PoolAllocator::PageSize() const
{
	return page_size;
}

inline unsigned int PoolAllocator::ObjectSize() const
{
	return memaddr_count_per_block * sizeof(unsigned int);
}

inline unsigned int PoolAllocator::MaxCount() const
{
	return pages.size() * blocks_per_page;
}

inline unsigned int PoolAllocator::UsedCount() const
{
	return MaxCount() - FreeCount();
}

inline unsigned int PoolAllocator::BlocksPerPage() const
{
	return blocks_per_page;
}

#define POOLALLOCATOR_DECL \
public: \
	static void * operator new(size_t size); \
	static void operator delete(void * p, size_t size); \
private: \
	static AutoPointer<PoolAllocator> memory_pool; \

#define POOLALLOCATOR_IMPL(MyClass, PageSize) \
AutoPointer<MemoryPool> TestObject::memory_pool; \
 \
void * MyClass::operator new(size_t size) \
{ \
	if (memory_pool == NULL) \
	{ \
		memory_pool = new MemoryPool(PageSize, sizeof(MyClass)); \
	} \
	return memory_pool->Alloc(); \
} \
void MyClass::operator delete(void * p, size_t size) \
{ \
	memory_pool->Free(p); \
} \

#endif