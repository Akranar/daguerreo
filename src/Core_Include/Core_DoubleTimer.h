#ifndef _TIMER_H_
#define _TIMER_H_

class DoubleTimerData;

class DoubleTimer
{
	DoubleTimerData * data;
	bool stopped;
public:
	DoubleTimer();
	~DoubleTimer();
	void StartTimer();
	void StopTimer();
	double GetTimeInSeconds();
};

#endif
