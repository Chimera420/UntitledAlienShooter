#pragma once
#include "raylib.h"
#include "Constants.h"

class Timer
{
public:
	Timer();
	~Timer();
	void startTimer(Timer* timer, float timeDuration);
	void updateTimer(Timer* timer);
	bool isTimerDone(Timer* timer);

private:
	float m_timerDuration;
};