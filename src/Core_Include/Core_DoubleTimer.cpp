#include "Core_DoubleTimer.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef _WIN32

//*************************************
//	Win32 Implementation
//*************************************

class DoubleTimerData
{
public:
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
};

DoubleTimer::DoubleTimer()
{
	stopped = false;
	data = new DoubleTimerData();
	this->data->start.QuadPart = 0;
	this->data->end.QuadPart = 0;
	QueryPerformanceFrequency(&this->data->frequency);
}

DoubleTimer::~DoubleTimer()
{
	delete data;
}

void DoubleTimer::StartTimer()
{
	QueryPerformanceCounter(&this->data->start);
	stopped = false;
}

void DoubleTimer::StopTimer()
{
	QueryPerformanceCounter(&this->data->end);
	stopped = true;
}

double DoubleTimer::GetTimeInSeconds()
{
	if (!stopped) QueryPerformanceCounter(&this->data->end);
	return double(data->end.QuadPart - data->start.QuadPart) / double(data->frequency.QuadPart);

}
#else

//*************************************
//	POSIX Implementation
//*************************************

class DoubleTimerData
{
public:
	timeval start;
	timeval end;
};

DoubleTimer::DoubleTimer()
{
	stopped = false;
	data = new DoubleTimerData();
	this->data->start.tv_sec = 0;
	this->data->start.tv_usec = 0;
	this->data->end.tv_sec = 0;
	this->data->end.tv_usec = 0;
}

DoubleTimer::~DoubleTimer()
{
	delete data;
}

void DoubleTimer::StartTimer()
{
	gettimeofday(&this->data->start, 0);
	stopped = false;
}

void DoubleTimer::StopTimer()
{
	gettimeofday(&this->data->end, 0);
	stopped = true;
}

double DoubleTimer::GetTimeInSeconds()
{
	if (!stopped) gettimeofday(&this->data->end, 0);
	return double(data->end.tv_sec - data->start.tv_sec)+(double(data->end.tv_usec - data->start.tv_usec) / double(1000000));
}
#endif
