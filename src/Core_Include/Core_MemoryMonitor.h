#ifndef MEMORY_MONITOR_H_
#define MEMORY_MONITOR_H_

#include <map>
#include <limits>
#include <iostream>

//#include <malloc.h>
#include <stdlib.h> //malloc.h is not found on macs

template<typename T>
class MallocAllocator
{
public : 
    //    typedefs

    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public : 
    //    convert an allocator<T> to allocator<U>

    template<typename U>
    struct rebind {
        typedef MallocAllocator<U> other;
    };

public : 
    inline explicit MallocAllocator() {}
    inline ~MallocAllocator() {}
    //inline explicit MallocAllocator(MallocAllocator const&) {}
	inline MallocAllocator(MallocAllocator const&) {} // IMPORTANT: GCC seems to hate the 'explicit' keyword in the commented out line.
    template<typename U>
    //inline explicit MallocAllocator(MallocAllocator<U> const&) {}
	inline MallocAllocator(MallocAllocator<U> const&) {}

    //    address

    inline pointer address(reference r) 
	{ 
		return &r; 
	}
    inline const_pointer address(const_reference r) 
	{ 
		return &r; 
	}

    //    memory allocation

    inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) 
	{
		return reinterpret_cast<pointer>(malloc (cnt * sizeof(T)));
		//return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T))); 
    }

    inline void deallocate(pointer p, size_type) 
	{ 
		free(p);
        //::operator delete(p); 
    }

    //    size

    inline size_type max_size() const 
	{
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}

    //    construction/destruction

    inline void construct(pointer p, const T& t) 
	{
		new(p) T(t); 
	}
    inline void destroy(pointer p) 
	{ 
		p->~T();
	}

    inline bool operator==(MallocAllocator const&) 
	{
		return true; 
	}
    inline bool operator!=(MallocAllocator const& a) 
	{ 
		return !operator==(a); 
	}

};    //    end of class Allocator 



typedef std::map<void *, int, std::less<void *>, MallocAllocator<std::pair<void *, int> > > MemoryRegistryMap;

class MemoryMonitor
{
private:
	static int allocation_id;
	static int num_allocations;
	static int num_deletions;
	static MemoryRegistryMap allocation_registry;
	static bool diagnosis_ready;
	static bool verbose;
	MemoryMonitor() {};
public:
	static void Increment();
	static void Decrement();
	static int * GetAllocCount();
	static int * GetDelCount();
	static void Register(void * alloc);
	static void DeRegister(void * alloc);
	static void DisplayMemoryCounts();
	static void Start();
	static void End();
	static bool Monitoring();
	static void DumpLeaks();
	static void SetVerbose(bool value);
};


#define MEMORYTRACK_START MemoryMonitor::Start();
#define MEMORYTRACK_END MemoryMonitor::End();
#define MEMORYTRACK_VERBOSE MemoryMonitor::SetVerbose(true);
#define MEMORYTRACK_SILENT MemoryMonitor::SetVerbose(false);





#endif
