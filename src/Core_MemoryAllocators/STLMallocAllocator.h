#ifndef _STL_MALLOC_ALLOCATOR_H_
#define _STL_MALLOC_ALLOCATOR_H_

#include <map>
#include <limits>
#include <iostream>

//#include <malloc.h>
#include <stdlib.h> //malloc.h not found on macs

template<typename T>
class STLMallocAllocator
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
    inline explicit MallocAllocator(MallocAllocator<U> const&) {}

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

#endif
