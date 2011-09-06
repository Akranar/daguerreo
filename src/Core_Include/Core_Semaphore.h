#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#if !defined(_TTHREAD_PLATFORM_DEFINED_)
  #if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define _TTHREAD_WIN32_
  #else
    #define _TTHREAD_POSIX_
  #endif
  #define _TTHREAD_PLATFORM_DEFINED_
#endif


#if defined(_TTHREAD_WIN32_)
#include <windows.h>
#else
#include <semaphore.h>
#endif

class Semaphore
{
#if defined(_TTHREAD_WIN32_)
    typedef HANDLE native_handle_type;
	unsigned int mWin32ThreadID;  
#else
    typedef sem_t native_handle_type;
#endif


	native_handle_type handle;
public:
	inline Semaphore(unsigned int initial_count, unsigned int max_count);
	inline ~Semaphore();
	inline void Decrement();
	inline void Increment();
	inline unsigned int GetCount();
};

#ifdef _WIN32

#include <windows.h>
#include <limits.h>

inline Semaphore::Semaphore(unsigned int initial_count, unsigned int max_count)
{
	handle = (native_handle_type) CreateSemaphore(NULL, initial_count, max_count, NULL);  
}
inline Semaphore::~Semaphore()
{
	CloseHandle((native_handle_type) handle);
}

inline void Semaphore::Decrement()
{
	WaitForSingleObject((native_handle_type) handle,INFINITE);
}

inline void Semaphore::Increment()
{
	ReleaseSemaphore((native_handle_type)handle,1,0);
}

inline unsigned int Semaphore::GetCount()
{
	long value = -1;
	ReleaseSemaphore((native_handle_type) handle,0,&value);
	return value;
}


#else

#endif

#endif