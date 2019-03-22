#pragma once

#include "MathCommon.h"
#include "Vec3f.h"
#include "Vec4f.h"
#include "Quaternion.h"

inline static vec3 extractScale(cmat4 _m)
{
	vec3 scale;
	scale.x = glm::length(vec3(_m[0][0], _m[0][1], _m[0][2])); // 1st column
	scale.y = glm::length(vec3(_m[1][0], _m[1][1], _m[1][2])); // 2nd column
	scale.z = glm::length(vec3(_m[2][0], _m[2][1], _m[2][2])); // 3rd columt
	return scale;
}