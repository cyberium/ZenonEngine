#pragma once

#include <cmath>

namespace Math
{
	const int MinInt32 = 0x80000000;
	const int MaxInt32 = 0x7FFFFFFF;

	const unsigned int MinUInt32 = 0x00000000;
	const unsigned int MaxUInt32 = 0xFFFFFFFF;

	const float MinFloat = -1000000000.0f; // FLT_MIN;
	const float MaxFloat = 1000000000.0f;

	const float Epsilon = 0.000001f;
	const float Epsilon100 = 0.0001f;
	const float Epsilon10000 = 0.01f;
	const float ZeroEpsilon = 32.0f * MinFloat;  // Very small epsilon for checking against 0.0f
	const float NaN = *(float*)&MaxUInt32;
};

inline glm::vec3 Fix_X0Z(const glm::vec3& _vec)
{
	return glm::vec3(_vec.x, 0.0f, _vec.z);
}

inline glm::vec3 extractScale(const glm::mat4& _m)
{
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3(_m[0][0], _m[0][1], _m[0][2])); // 1st column
	scale.y = glm::length(glm::vec3(_m[1][0], _m[1][1], _m[1][2])); // 2nd column
	scale.z = glm::length(glm::vec3(_m[2][0], _m[2][1], _m[2][2])); // 3rd columt
	return scale;
}
