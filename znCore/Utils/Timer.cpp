#include "stdafx.h"

// General
#include "Timer.h"

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	m_Previous = std::clock();
}

float Timer::GetElapsedTime() const
{
	float currentTime = std::clock();
	float deltaTime = currentTime - m_Previous;
	m_Previous = currentTime;
	return deltaTime;
}
