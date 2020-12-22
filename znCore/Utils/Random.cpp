#include "stdafx.h"

// General
#include "Random.h"


// Mersene Twister 19937 generator (64 bit) 
std::mt19937_64 gRandomGenerator;
std::uniform_int_distribution<int32> Random::m_IntDistribution;
std::uniform_int_distribution<uint32> Random::m_UIntDistribution;
std::uniform_real_distribution<float> Random::m_FloatDistribution;

void Random::Initialize()
{
	gRandomGenerator.seed(std::time(nullptr));
}

int32_t Random::NextInt()
{
    return m_IntDistribution(gRandomGenerator);
}

uint32_t Random::NextUInt()
{
    return m_UIntDistribution(gRandomGenerator);
}

float Random::NextFloat()
{
    return m_FloatDistribution(gRandomGenerator);
}

float Random::Range(const float min, const float max)
{
    return NextFloat() * (max - min) + min;
}

glm::vec2 Random::NextVec2f()
{
    return glm::vec2(NextFloat(), NextFloat());
}

glm::vec3 Random::NextVec3f()
{
    return glm::vec3(NextFloat(), NextFloat(), NextFloat());
}

glm::vec2 Random::UnitVector2f()
{
    float angle = NextFloat() * glm::two_pi<float>();
    return glm::vec2(std::cos(angle), std::sin(angle));
}

glm::vec3 Random::UnitVector3f()
{
    float z = Range(-1.0f, 1.0f);
	glm::vec2 disc = UnitVector2f() * std::sqrt(1.0f - (z*z));
    return glm::vec3(disc.x, disc.y, z);
}

glm::vec3 Random::Hemisphere(const glm::vec3& normal)
{
    // 1. Generate a random unit vector in a sphere
	glm::vec3 unitVector = UnitVector3f();

    // Negate it if it is pointing away from the normal
    if (glm::dot(unitVector, normal) < 0.0f)
    {
        unitVector = -unitVector;
    }

    return unitVector;
}