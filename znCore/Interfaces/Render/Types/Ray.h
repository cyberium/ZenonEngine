#pragma once;

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
// FORWARD END

struct Ray
{
	Ray()
	{}
	Ray(vec3 origin, vec3 direction)
		: m_Origin(origin)
		, m_Direction(direction)
	{}

	// Gets a point that is distance units along the ray.
	vec3 GetPointOnRay(float distance) const
	{
		return m_Origin + (m_Direction * distance);
	}

	// The origin of the ray in 3D space.
	vec3 m_Origin;

	// The normalized direction of the ray in 3D space.
	vec3 m_Direction;

	// The material that was hit.
	IMaterial* m_pMaterial;
};