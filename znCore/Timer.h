#pragma once;

/**
 * A simple low-resolution (float) timer class.
 */
class Timer
{
public:
	/**
	 * Constructor.
	 * @param maxTimeStep: Clamp to the maximum time step when the GetElapsedTime function is queried.
	 * This is useful when debugging and we don't want to have massive time-steps thus breaking the application.
	 */
	Timer();

	/**
	 * Query the elapsed time since either, the object was created or,
	 * this function was called. The counter is reset when this function is called
	 * so only call it once per frame and store the result.
	 * @returns: The elapsed time measured in seconds.
	 */
	float GetElapsedTime() const;

private:
	mutable float m_fPrevious;
};
