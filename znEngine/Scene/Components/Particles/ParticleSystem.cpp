#include "stdafx.h"

// General
#include "ParticleSystem.h"



template<class T>
inline T lifeRamp(float currentTime, float mid, const T& a, const T& b, const T& c)
{
	if (currentTime <= mid)
		return interpolateLinear<T>(currentTime / mid, a, b);
	else
		return interpolateLinear<T>((currentTime - mid) / (1.0f - mid), b, c);
}