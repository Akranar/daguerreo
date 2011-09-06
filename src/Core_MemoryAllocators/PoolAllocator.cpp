#include "PoolAllocator.h"
#include <iostream>
#include <Core_Exception.h>

#define MEMORY_ADDRESS void *

PoolAllocator::PoolAllocator(size_t page_size, size_t allocation_size)
: 
page_size(page_size), 
memaddr_count_per_block(allocation_size / sizeof(void *) + (allocation_size % sizeof(void *) == 0 ? 0 : 1)), 
blocks_available(0)

{
	blocks_per_page = page_size / (memaddr_count_per_block * sizeof(MEMORY_ADDRESS));
	pages.reserve(4);
	pages.push_back((MEMORY_ADDRESS *) ::operator new (page_size));
	ConfigurePage((MEMORY_ADDRESS *)pages.back());
	free_list = pages.back();
}
PoolAllocator::~PoolAllocator()
{
#ifdef _DEBUG
	if (blocks_available < blocks_per_page * pages.size())
	{
		std::cout << "MEMORY LEAK DETECTED IN POOL" << std::endl;
		getchar();
	}
#endif
	FreeAll();
}


void * PoolAllocator::Allocate(unsigned int size)
{
	MEMORY_ADDRESS * rtn;
	if (blocks_available == 0)
	{
		FetchPage();
		free_list = pages.back();
	}
	rtn = free_list;
	free_list = (MEMORY_ADDRESS *) free_list[0];
	--blocks_available;
	return rtn;
	
}
void PoolAllocator::Deallocate(void * p)
{
	if (blocks_available <= 0)
	{
		free_list = (MEMORY_ADDRESS *) p;
		++blocks_available;
		return;
	}
	MEMORY_ADDRESS * p_address_ptr = (MEMORY_ADDRESS *) p;
	p_address_ptr[0] = (MEMORY_ADDRESS) free_list;
	free_list = p_address_ptr;
	++blocks_available;
}



void PoolAllocator::ConfigurePage(MEMORY_ADDRESS * page)
{
	MEMORY_ADDRESS * page_current = page;
	blocks_available += blocks_per_page;
	for (unsigned int i = 0; i < blocks_per_page; ++i)
	{
		MEMORY_ADDRESS value = (MEMORY_ADDRESS) &page_current[memaddr_count_per_block];
		page_current[0] = value;
		page_current += memaddr_count_per_block;
	}
}

void PoolAllocator::FetchPage()
{
	pages.push_back((MEMORY_ADDRESS *) ::operator new (page_size));
	ConfigurePage((MEMORY_ADDRESS *)pages.back());
}

void PoolAllocator::FreeAll()
{
	blocks_available = 0;
	unsigned int num_pages = pages.size();
	for (unsigned int i = 0; i < num_pages; ++i)
	{
		::operator delete (pages[i]);
	}
	pages.clear();
}

