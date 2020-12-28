#pragma once

#include <cmath>
#include "../Debug.h"

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

inline void DecomposeMatrix(const glm::mat4& Matrix, glm::vec3 * Position = nullptr, glm::quat * Rotation = nullptr, glm::vec3 * Scale = nullptr)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	if (false == glm::decompose(Matrix, scale, rotation, translation, skew, perspective))
		throw CException("Unable to decompose matrix.");

	if (Position != nullptr)
		*Position = translation;

	if (Rotation != nullptr)
		*Rotation = rotation;

	if (Scale != nullptr)
		*Scale = scale;
}

inline glm::vec3 DecomposePositionMatrix(const glm::mat4& Matrix)
{
	glm::vec3 position;
	DecomposeMatrix(Matrix, &position);
	return position;
}

inline glm::quat DecomposeRotationMatrix(const glm::mat4& Matrix)
{
	glm::quat rotation;
	DecomposeMatrix(Matrix, nullptr, &rotation);
	return rotation;
}

inline glm::vec3 DecomposeScaleMatrix(const glm::mat4& Matrix)
{
	glm::vec3 scale;
	DecomposeMatrix(Matrix, nullptr, nullptr, &scale);
	return scale;
}

inline glm::vec3 QuatToEuler(const glm::quat& Quat)
{
	float yaw, pitch, roll;
	glm::extractEulerAngleXYZ(glm::toMat4(Quat), yaw, pitch, roll);

	return glm::vec3(yaw, pitch, roll);
}



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
