#pragma once

inline static glm::vec3 extractScale(const glm::mat4& _m)
{
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3(_m[0][0], _m[0][1], _m[0][2])); // 1st column
	scale.y = glm::length(glm::vec3(_m[1][0], _m[1][1], _m[1][2])); // 2nd column
	scale.z = glm::length(glm::vec3(_m[2][0], _m[2][1], _m[2][2])); // 3rd columt
	return scale;
}