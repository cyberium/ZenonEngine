#pragma once

struct RaycastHit
{
	// The point in 3D space where the ray hit the geometry.
	vec3 Point;

	// The surface normal where the ray hit the geometry.
	vec3 Normal;

	// The distance from the ray origin to the impact point.
	float Distance;

	// A pointer to the material that was hit (if one was, NULL otherwise)
	IMaterial* pMaterial;
};