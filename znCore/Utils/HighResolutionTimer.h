#pragma once

class ZN_API HighResolutionTimer
{
public:
	HighResolutionTimer()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	double GetElapsedMilliSeconds() const
	{
		return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_Start).count();
	}
	double GetElapsedMicroSeconds() const
	{
		return std::chrono::duration<double, std::micro>(std::chrono::high_resolution_clock::now() - m_Start).count();
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
