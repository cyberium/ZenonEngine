#pragma once;

class Timer
{
public:
	Timer();

	void Reset();
	float GetElapsedTime() const;

private:
	mutable float m_Previous;
};
