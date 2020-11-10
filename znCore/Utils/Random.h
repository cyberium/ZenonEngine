#pragma once

/**
 * Random number generator.
 * Makes use of the random classes in the C++11 standard template library.
 */

#include <random>

class ZN_API Random
{
public:
    // Initialize the random generator with an initial seed. Default is 0. 
    // If you require non-deterministic random-number generator,
    // Then seed should be something like the current time.
    // Consider using std::time() to seed the random number generator.
    static void Initialize();

    // Generate a random number.
    static int32_t NextInt();

    // Generate a non-negative random number.
	static uint32_t NextUInt();

    // Generates a random float in the range [0, 1]
	static float NextFloat();

    // Generates a random number in the specified range
	static float Range(const float min, const float max);

    // Generates a random vec2 (non-normalized)
    // Each component is in the range [0, 1].
	static glm::vec2 NextVec2f();

    // Generates a random vec3 (non-normalized)
    // Each component is in the range [0, 1].
	static glm::vec3 NextVec3f();

    // Generate a 2D random unit vector.
	static glm::vec2 UnitVector2f();

    // Generate a 3D random unit vector.
	static glm::vec3 UnitVector3f();

    // Generate a random unit vector in the hemisphere 
    // of the passed-in normal vector.
	static glm::vec3 Hemisphere(const glm::vec3& normal);

private:


	static std::uniform_int_distribution<int32> m_IntDistribution;
	static std::uniform_int_distribution<uint32> m_UIntDistribution;
	static std::uniform_real_distribution<float> m_FloatDistribution;
};