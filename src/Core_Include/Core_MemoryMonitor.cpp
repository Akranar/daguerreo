#include "Core_MemoryMonitor.h"
//#include <malloc.h>
#include <stdlib.h> //malloc.h is not found on macs
#include <cstdio>



#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
//***************************************************************
#include <windows.h>
struct Mutex
{
	CRITICAL_SECTION mutex;
	Mutex() {InitializeCriticalSection(&mutex);}
	void Lock(){EnterCriticalSection(&mutex);}
	void Unlock(){LeaveCriticalSection(&mutex);}
};
//***************************************************************
#else
//***************************************************************
#include <pthread.h>

struct Mutex
{
	pthread_mutex_t mutex;
	Mutex() {pthread_mutex_init(&mutex, NULL);}
	void Lock(){pthread_mutex_lock(&mutex);}
	void Unlock(){pthread_mutex_unlock(&mutex);}
};
//***************************************************************
#endif

struct ScopedLock
{
	Mutex & mutex;
	ScopedLock(Mutex & mutex)
		: mutex(mutex)
	{
		mutex.Lock();
	}

	~ScopedLock()
	{
		mutex.Unlock();
	}
};



int MemoryMonitor::allocation_id = 0;
int MemoryMonitor::num_allocations = 0;
int MemoryMonitor::num_deletions = 0;
MemoryRegistryMap MemoryMonitor::allocation_registry = MemoryRegistryMap();
bool MemoryMonitor::diagnosis_ready = false;
bool MemoryMonitor::verbose = false;
Mutex mutex;



void MemoryMonitor::Increment()
{
	++num_allocations;
	if (verbose) 
	{
		std::printf("Alloc (%d): %d\n", allocation_id, num_allocations - num_deletions);
		std::fflush (stdout);
	}

}

void MemoryMonitor::Decrement()
{
	++num_deletions;
	if (verbose) std::printf("Del: %d\n", num_allocations - num_deletions);
}

int * MemoryMonitor::GetAllocCount()
{
	return &num_allocations;
}
int * MemoryMonitor::GetDelCount()
{
	return &num_deletions;
}

void MemoryMonitor::DisplayMemoryCounts()
{
	std::printf("Allocations: %d\n", num_allocations);
	std::printf("Deletions: %d\n", num_deletions);
	std::printf("	: %d\n", num_allocations - num_deletions);
}

void MemoryMonitor::Register(void * alloc)
{
	allocation_registry.insert(std::make_pair(alloc, allocation_id));
	allocation_id += 1;
}
void MemoryMonitor::DeRegister(void * alloc)
{
	ScopedLock lock(mutex);
	allocation_registry.erase(alloc);
}

void MemoryMonitor::Start()
{
	ScopedLock lock(mutex);
	diagnosis_ready = true;
}
void MemoryMonitor::End()
{
	ScopedLock lock(mutex);
	diagnosis_ready = false;
	DumpLeaks();
	allocation_registry.clear();
}
void MemoryMonitor::SetVerbose(bool value)
{
	ScopedLock lock(mutex);
	verbose = value;
}
bool MemoryMonitor::Monitoring()
{
	ScopedLock lock(mutex);
	return diagnosis_ready;
}

void MemoryMonitor::DumpLeaks()
{
	if (allocation_registry.size() == 0) return;
	std::printf("MEMORY LEAKS DETECTED: %d\n", allocation_registry.size());
	for (MemoryRegistryMap::iterator iter = allocation_registry.begin(); iter != allocation_registry.end(); ++iter)
	{
		std::printf("	%d\n", iter->second);
	}
	getchar();
}

/*
	NOTE: Must use c-style prints inside overridden NEW and DELETE
		-> C++ cout style will manages its own dynamic memory.
			-> Will end up calling new etc.
				-> Bad.
*/


#ifdef _DEBUG

void * operator new (size_t size)
{
	
	void * allocation = malloc(size);
	if (MemoryMonitor::Monitoring())
	{
		ScopedLock lock(mutex);
		MemoryMonitor::Increment();
		MemoryMonitor::Register(allocation);
	}
	
	return allocation;
};

void operator delete (void * to_delete)
{
	
	if (MemoryMonitor::Monitoring())
	{
		ScopedLock lock(mutex);
		MemoryMonitor::Decrement();
		MemoryMonitor::DeRegister(to_delete);
	}
	free(to_delete);
}

void * operator new[] (size_t size)
{
	
	void * allocation = malloc(size);
	if (MemoryMonitor::Monitoring())
	{
		ScopedLock lock(mutex);
		MemoryMonitor::Increment();
		MemoryMonitor::Register(allocation);
		
	}
	
	return allocation;
};

void operator delete[] (void * to_delete)
{
	if (MemoryMonitor::Monitoring())
	{
		ScopedLock lock(mutex);
		MemoryMonitor::Decrement();
		MemoryMonitor::DeRegister(to_delete);
		
	}
	free(to_delete);
}


#endif


