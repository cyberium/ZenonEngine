#pragma once

class Plane
{
public:
	Plane()
	{
		normal.x = 0; normal.y = 0; normal.z = 0; dist = 0;
	};

	explicit Plane(const float a, const float b, const float c, const float d)
	{
		normal = vec3(a, b, c);
		float invLen = 1.0f / glm::length(normal);
		normal *= invLen;	// Normalize
		dist = d * invLen;
	}

	Plane(cvec3 c, cvec3 b, cvec3 a)
	{
		vec3 cb = b - c;
		vec3 ca = a - c;
		normal = glm::cross(cb, ca);
		normal = glm::normalize(normal);
		dist = - glm::dot(normal, c);
	}

	// ----------------
	// Other operations
	// ----------------
	float distToPoint(cvec3 v) const
	{
		return glm::dot(normal, v) + dist;
	}

public:
	vec3 normal;
	float dist;
};

inline bool cullBoxByPlanes(const Plane* _planes, uint32 _planesCount, const BoundingBox& b)
{
	// Idea for optimized AABB testing from www.lighthouse3d.com
	for (uint32 i = 0; i < _planesCount; ++i)
	{
		cvec3 n = _planes[i].normal;

		vec3 positive = b.getMin();
		if (n.x <= 0) positive.x = b.getMax().x;
		if (n.y <= 0) positive.y = b.getMax().y;
		if (n.z <= 0) positive.z = b.getMax().z;

		if (_planes[i].distToPoint(positive) > 0)
		{
			return true;
		}
	}

	return false;
}

inline bool cullPolyByPlanes(const Plane* _planes, uint32 _planesCount, const vec3* verts, uint32 vertsCount)
{
	for (uint32 i = 0; i < _planesCount; ++i)
	{
		bool allOut = true;

		for (uint32 j = 0; j < vertsCount; ++j)
		{
			if (_planes[i].distToPoint(verts[j]) < 0)
			{
				allOut = false;
				break;
			}
		}

		if (allOut) return true;
	}

	return false;
}