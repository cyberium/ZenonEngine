#pragma once

#include "MathCommon.h"

static inline vec3 Fix_X0Z(cvec3 _vec)
{
	return vec3(_vec.x, 0.0f, _vec.z);
}

static inline vec3 Fix_XZY(cvec3 _vec)
{
	return vec3(_vec.x, _vec.z, _vec.y);
}

static inline vec3 Fix_XZmY(cvec3 _vec)
{
	return vec3(_vec.x, _vec.z, -_vec.y);
}
