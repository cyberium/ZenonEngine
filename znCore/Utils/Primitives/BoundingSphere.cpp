#include "stdafx.h"

// General
#include "BoundingSphere.h"

inline float InvSqr(float v)
{
	return (v != 0.0f) ? 1.0f / (v * v) : 0.0f;
}

BoundingSphere::BoundingSphere(const glm::vec3& Center, float Radius)
	: m_Center(Center)
	, m_Radius(Radius)
{
}

const glm::vec3& BoundingSphere::GetCenter() const
{
	return m_Center;
}

float BoundingSphere::GetRadius() const
{
	return m_Radius;
}

bool BoundingSphere::IsInfinite() const
{
	return m_Radius == Math::MaxFloat;
}
