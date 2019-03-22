#include "stdafx.h"

#include "NEW_Random.h"

NEW_Random::NEW_Random(int seed)
	: m_RandomGenerator(seed)
{}

int32_t NEW_Random::NextInt() const
{
	return m_IntDistribution(m_RandomGenerator);
}

uint32_t NEW_Random::NextUInt() const
{
	return m_UIntDistribution(m_RandomGenerator);
}

float NEW_Random::NextFloat() const
{
	return m_FloatDistribution(m_RandomGenerator);
}

float NEW_Random::Range(const float min, const float max) const
{
	return NextFloat() * (max - min) + min;
}

vec2 NEW_Random::NextVec2f() const
{
	return vec2(NextFloat(), NextFloat());
}

vec3 NEW_Random::NextVec3f() const
{
	return vec3(NextFloat(), NextFloat(), NextFloat());
}

vec2 NEW_Random::UnitVector2f() const
{
	float angle = NextFloat() * 2.0f * glm::pi<float>();
	return vec2(std::cos(angle), std::sin(angle));
}

vec3 NEW_Random::UnitVector3f() const
{
	float z = Range(-1, 1);
	vec2 disc = UnitVector2f() * std::sqrt(1.0f - (z*z));
	return vec3(disc.x, disc.y, z);
}

vec3 NEW_Random::Hemisphere(cvec3 normal)
{
	// 1. Generate a random unit vector in a sphere
	vec3 unitVector = UnitVector3f();
	// Negate it if it is pointing away from the normal
	if (glm::dot(unitVector, normal) < 0.0f)
	{
		unitVector = -unitVector;
	}

	return unitVector;
}