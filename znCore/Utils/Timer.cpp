#include "stdafx.h"

// General
#include "Timer.h"

Timer::Timer()
	: m_fPrevious(std::clock())
{}

float Timer::GetElapsedTime() const
{
	float fCurrentTime = std::clock();
	float fDeltaTime = fCurrentTime - m_fPrevious;
	m_fPrevious = fCurrentTime;

	return fDeltaTime;
}
