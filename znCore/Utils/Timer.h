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
	float GetElapsedTime() const
	{
		float currentTime = std::clock();
		float deltaTime = currentTime - m_Previous;
		m_Previous = currentTime;
		return deltaTime;
	}

private:
	mutable float m_Previous;
};
