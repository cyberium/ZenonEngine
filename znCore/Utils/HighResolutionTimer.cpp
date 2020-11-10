#include "stdafx.h"

#include "HighResolutionTimer.h"

HighResolutionTimer::HighResolutionTimer()
{
	m_Start = std::chrono::high_resolution_clock::now();
}

double HighResolutionTimer::GetElapsedMilliSeconds() const
{
	return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_Start).count();
}

double HighResolutionTimer::GetElapsedMicroSeconds() const
{
	return std::chrono::duration<double, std::micro>(std::chrono::high_resolution_clock::now() - m_Start).count();
}