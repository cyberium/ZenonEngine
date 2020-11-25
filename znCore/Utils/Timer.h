#pragma once;

class ZN_API Timer
{
public:
	Timer()
	{
		Reset();
	}

	void Reset()
	{
		m_Previous = std::clock();
	}
	long GetElapsedTime() const
	{
		long currentTime = std::clock();
		long deltaTime = currentTime - m_Previous;
		m_Previous = currentTime;
		return deltaTime;
	}

private:
	mutable long m_Previous;
};
