#include "Clock.h"
#include "GameException.h"

LARGE_INTEGER Clock::m_time;
LARGE_INTEGER Clock::m_startTime;
LARGE_INTEGER Clock::m_timerFreq;

bool Clock::Initialize()
{
	if (!QueryPerformanceFrequency(&m_timerFreq))
	{
		throw(GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing game counter"));
	}

	QueryPerformanceCounter(&Clock::m_startTime);
}
