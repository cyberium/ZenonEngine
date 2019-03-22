#pragma once

class Timer_OLD
{
public:

	Timer_OLD() : _elapsedTime(0), _startTime(0), _enabled(false)
	{
		// Find first available CPU
		DWORD_PTR procMask, sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);
		_affMask = 1;
		while ((_affMask & procMask) == 0) _affMask <<= 1;

		// Get timer frequency
		DWORD_PTR threadAffMask = SetThreadAffinityMask(GetCurrentThread(), _affMask);
		QueryPerformanceFrequency(&_timerFreq);
		SetThreadAffinityMask(GetCurrentThread(), threadAffMask);
	}

	void setEnabled(bool enabled)
	{
		if (enabled && !_enabled)
		{
			_startTime = getTime();
			_enabled = true;
		}
		else if (!enabled && _enabled)
		{
			double endTime = getTime();
			_elapsedTime += endTime - _startTime;
			_enabled = false;
		}
	}

	void reset()
	{
		_elapsedTime = 0;
		if (_enabled) _startTime = getTime();
	}

	float getElapsedTimeMS()
	{
		if (_enabled)
		{
			double endTime = getTime();
			_elapsedTime += endTime - _startTime;
			_startTime = endTime;
		}

		return (float)_elapsedTime;
	}

protected:

	double getTime() const
	{
		// Make sure that time is read from the same CPU
		DWORD_PTR threadAffMask = SetThreadAffinityMask(GetCurrentThread(), _affMask);

		// Read high performance counter
		LARGE_INTEGER curTick;
		QueryPerformanceCounter(&curTick);

		// Restore affinity mask
		SetThreadAffinityMask(GetCurrentThread(), threadAffMask);

		return (double)curTick.QuadPart / (double)_timerFreq.QuadPart * 1000.0;

	}

protected:
	double         _startTime;
	double         _elapsedTime;

	LARGE_INTEGER  _timerFreq;
	DWORD_PTR      _affMask;

	bool           _enabled;
};
