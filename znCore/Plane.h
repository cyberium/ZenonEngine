#pragma once

class ZN_API Plane
{
public:
	Plane()
		: normal(glm::vec3(0.0f))
		, dist(0.0f)
	{};

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
	glm::vec3 normal;
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


inline bool cullPolyByPlanes(const Plane* Planes, size_t PlanesCount, const glm::vec3* Vertices, size_t VerticesCount)
{
	for (size_t i = 0; i < PlanesCount; ++i)
	{
		bool allOut = true;

		for (size_t j = 0; j < VerticesCount; j++)
		{
			if (Planes[i].distToPoint(Vertices[j]) < 0)
			{
				allOut = false;
				break;
			}
		}

		if (allOut) 
			return true;
	}

	return false;
}

inline bool cullPolyByPlanes(const Plane* Planes, size_t PlanesCount, const std::vector<glm::vec3>& PolyVertices)
{
	return cullPolyByPlanes(Planes, PlanesCount, PolyVertices.data(), PolyVertices.size());
}