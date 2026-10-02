#include "Timer.h"

Timer::Timer() :
	m_timerDuration()
{

}

Timer::~Timer()
{

}

void Timer::startTimer(Timer* timer, float timeDuration)
{
	if (timer != NULL)
	{
		timer->m_timerDuration = timeDuration;
	}
}

void Timer::updateTimer(Timer* timer)
{
	if (timer != NULL && timer->m_timerDuration > 0)
	{
		timer->m_timerDuration -= GetFrameTime();
	}
}

bool Timer::isTimerDone(Timer* timer)
{
	if (timer != NULL)
	{
		return timer->m_timerDuration <= 0;
	}
	return true;
}
