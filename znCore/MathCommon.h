#pragma once

#include <cmath>

namespace Math
{
	const int MinInt32 = 0x80000000;
	const int MaxInt32 = 0x7FFFFFFF;

	const unsigned int MinUInt32 = 0x00000000;
	const unsigned int MaxUInt32 = 0xFFFFFFFF;

	const float MinFloat = 1.175494351e-38F;
	const float MaxFloat = 3.402823466e+38F;

	const float Epsilon = 0.000001f;
	const float Epsilon100 = 0.0001f;
	const float Epsilon10000 = 0.01f;
	const float ZeroEpsilon = 32.0f * MinFloat;  // Very small epsilon for checking against 0.0f
	const float NaN = *(float*)&MaxUInt32;

	enum NoInitHint
	{
		NO_INIT
	};
};

// -------------------------------------------------------------------------------------------------
// General
// -------------------------------------------------------------------------------------------------

static inline float clamp(float f, float min, float max)
{
	if (f < min) f = min;
	else if (f > max) f = max;

	return f;
}

static inline float minf(float a, float b)
{
	return a < b ? a : b;
}

static inline float maxf(float a, float b)
{
	return a > b ? a : b;
}

static inline float fsel(float test, float a, float b)
{
	// Branchless selection
	return test >= 0 ? a : b;
}

static inline void rotate(float x0, float y0, float *x, float *y, float angle)
{
	float xa = *x - x0;
	float ya = *y - y0;
	*x = xa * cosf(angle) - ya * sinf(angle) + x0;
	*y = xa * sinf(angle) + ya * cosf(angle) + y0;
}

static inline int ftoi_t(double val)
{
	// Float to int conversion using truncation

	return (int)val;
}

static inline int ftoi_r(double val)
{
	// Fast round (banker's round) using Sree Kotay's method
	// This function is much faster than a naive cast from float to int

	union
	{
		double dval;
		int ival[2];
	} u;

	u.dval = val + 6755399441055744.0;  // Magic number: 2^52 * 1.5;
	return u.ival[0];         // Needs to be [1] for big-endian
}
