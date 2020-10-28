#pragma once

class HighResolutionTimer
{
public:
	HighResolutionTimer();

	float GetElapsedMilliSeconds() const;
	float GetElapsedMicroSeconds() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
