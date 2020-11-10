#pragma once

class HighResolutionTimer
{
public:
	HighResolutionTimer();

	double GetElapsedMilliSeconds() const;
	double GetElapsedMicroSeconds() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
