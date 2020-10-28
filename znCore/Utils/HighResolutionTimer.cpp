#include "stdafx.h"

#include "HighResolutionTimer.h"

HighResolutionTimer::HighResolutionTimer()
{
	m_Start = std::chrono::high_resolution_clock::now();
}

float HighResolutionTimer::GetElapsedMilliSeconds() const
{
	return std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - m_Start).count();
}

float HighResolutionTimer::GetElapsedMicroSeconds() const
{
	return std::chrono::duration<float, std::micro>(std::chrono::high_resolution_clock::now() - m_Start).count();
}