#include "stdafx.h"

// General
#include "Ray.h"


Ray::Ray()
{}

Ray::Ray(vec3 origin, vec3 direction)
	: m_Origin(origin)
	, m_Direction(direction)
{}

vec3 Ray::GetPointOnRay(float distance) const
{
	return m_Origin + (m_Direction * distance);
}