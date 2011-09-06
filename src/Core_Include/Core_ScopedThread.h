#ifndef _SCOPED_THREAD_H_
#define _SCOPED_THREAD_H_

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
#include <process.h>
#else
#include <pthread.h>
#endif


/*
template <class FUNCTOR>
class ScopedThread
{
#if defined(_TTHREAD_WIN32_)
    typedef HANDLE native_handle_type;
	unsigned int mWin32ThreadID;  
#else
    typedef pthread_t native_handle_type;
#endif

	native_handle_type handle;
	FUNCTOR functor;
public:

#if defined(_TTHREAD_WIN32_)
	static unsigned WINAPI ScopedThread_Entry(void * arg)
	{
		((ScopedThread<FUNCTOR> *) arg)->functor();
		return 0;
	}
#else
	 static void * ScopedThread_Entry(void * arg)
	{
		((ScopedThread<FUNCTOR> *) arg)->functor();
		return 0;
	}
#endif
	ScopedThread(const FUNCTOR & functor)
		: functor(functor)
	{
#if defined(_TTHREAD_WIN32_)
		handle = (HANDLE) _beginthreadex(0, 0, ScopedThread_Entry, (void *) this, 0, &mWin32ThreadID);
#else
		if(pthread_create(&handle, NULL, ScopedThread_Entry, (void *) this) != 0)
			handle = 0;
#endif
	}
	ScopedThread()
		: handle(0)
	{
	}
	void SetFunc(const FUNCTOR & in_functor)
	{
		functor = in_functor;
#if defined(_TTHREAD_WIN32_)
		handle = (HANDLE) _beginthreadex(0, 0, wrapper_function, (void *) &functor, 0, &mWin32ThreadID);
#else
		if(pthread_create(&handle, NULL, ScopedThread_Entry, (void *) &functor) != 0)
			handle = 0;
#endif
	}
	~ScopedThread()
	{
		if (handle)
		{
			Join();
			//Windows_CloseHandle(thread);
		}
	}
	void Join()
	{
#if defined(_TTHREAD_WIN32_)
		WaitForSingleObject(handle, INFINITE);
#elif defined(_TTHREAD_POSIX_)
		pthread_join(handle, NULL);
#endif

	}
};
*/

#if defined(_TTHREAD_WIN32_)

template <class FUNCTOR>
class ScopedThread
{

    typedef HANDLE native_handle_type;
	unsigned int mWin32ThreadID;  

	native_handle_type handle;
	FUNCTOR functor;
public:

	static unsigned WINAPI ScopedThread_Entry(void * arg)
	{
		((ScopedThread<FUNCTOR> *) arg)->functor();
		return 0;
	}

	ScopedThread(const FUNCTOR & functor)
		: functor(functor)
	{
		handle = (HANDLE) _beginthreadex(0, 0, ScopedThread_Entry, (void *) this, 0, &mWin32ThreadID);
	}
	ScopedThread()
		: handle(0)
	{
	}
	void SetFunc(const FUNCTOR & in_functor)
	{
		functor = in_functor;
		handle = (HANDLE) _beginthreadex(0, 0, ScopedThread_Entry, (void *) this, 0, &mWin32ThreadID);
	}
	~ScopedThread()
	{
		if (handle)
		{
			Join();
			//Windows_CloseHandle(thread);
		}
	}
	void Join()
	{
		WaitForSingleObject(handle, INFINITE);
	}
};

#endif




#endif