#pragma once

#include <windows.h>

class Clock
{

public:

	static bool Initialize();

	static double GameTimeMillis()
	{
		QueryPerformanceCounter(&m_time);
		return (double)(m_time.QuadPart - m_startTime.QuadPart) / 
			(double)m_timerFreq.QuadPart * 1000.0;
	}

	static double GameTimeSeconds()
	{
		QueryPerformanceCounter(&m_time);
		return (double)(m_time.QuadPart - m_startTime.QuadPart) / 
			(double)m_timerFreq.QuadPart;
	}

	static long double GetTimeMillis()
	{
		QueryPerformanceCounter(&m_time);
		return (long double)m_time.QuadPart / (long double)m_timerFreq.QuadPart * 
			(long double)1000.0;
	}

	static double GetTimeSeconds()
	{
		QueryPerformanceCounter(&m_time);
		return (double)m_time.QuadPart / (double)m_timerFreq.QuadPart;
	}

private:

	static LARGE_INTEGER m_time;

	/* Time of clock initialization */
	static LARGE_INTEGER m_startTime;

	/* Timer counts per second */
	static LARGE_INTEGER m_timerFreq;

	Clock() {}
	~Clock() {}

};

