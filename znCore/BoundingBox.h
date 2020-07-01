#pragma once

// Include
#include "Math.h"

class ZN_API BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const glm::vec3& Min, const glm::vec3& Max);

	BoundingBox& operator=(const BoundingBox& _other)
	{
		m_Min = _other.m_Min;
		m_Max = _other.m_Max;
		m_Center = _other.m_Center;
		m_Radius = _other.m_Radius;
		m_IsCenterCalc = _other.m_IsCenterCalc;
		return *this;
	}

	void set(const glm::vec3& Min, const glm::vec3& Max);
	void calculate(const glm::vec3* _verts, uint32 _count);
	void calculateCenter();
	void clear();
	bool isClear() const;

	//

	void setMin(const glm::vec3& _min) { m_Min = _min; m_IsCenterCalc = false; }
	void setMinY(float value) { m_Min.y = value; m_IsCenterCalc = false; }
	const glm::vec3& getMin() const { return m_Min; }

	void setMax(const glm::vec3& _max) { m_Max = _max; m_IsCenterCalc = false; }
	void setMaxY(float value) { m_Max.y = value; m_IsCenterCalc = false; }
	const glm::vec3& getMax() const { return m_Max; }

	const glm::vec3& getCenter() const { _ASSERT(m_IsCenterCalc); return m_Center; }
	float getRadius() const { _ASSERT(m_IsCenterCalc); return m_Radius; }

	glm::vec3 getCorner(uint32 index) const;
	void transform(const glm::mat4& m);
	bool makeUnion(const BoundingBox& b);
	bool isPointInside(const glm::vec3& _point) const;

	inline void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
	{
		ByteBuffer->read(&m_Min);
		ByteBuffer->read(&m_Max);
		calculateCenter();
	}

	inline void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
	{
		ByteBuffer->write(&m_Min);
		ByteBuffer->write(&m_Max);
	}

private:
	glm::vec3 m_Min;
	glm::vec3 m_Max;
	glm::vec3 m_Center;
	float m_Radius;
	bool m_IsCenterCalc;

private:

};


// -------------------------------------------------------------------------------------------------
// Intersection
// -------------------------------------------------------------------------------------------------

#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

inline char HitBoundingBox(glm::vec3 minB, glm::vec3 maxB, glm::vec3 origin, glm::vec3 dir)
{
	char inside = TRUE;
	char quadrant[NUMDIM];
	register int i;
	int whichPlane;
	double maxT[NUMDIM];
	double candidatePlane[NUMDIM];
	
	/* Find candidate planes; this loop can be avoided if
	rays cast all from the eye(assume perpsective view) */
	for (i = 0; i < NUMDIM; i++)
	{
		if (origin[i] < minB[i])
		{
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = FALSE;
		}
		else if (origin[i] > maxB[i])
		{
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = FALSE;
		}
		else
		{
			quadrant[i] = MIDDLE;
		}
	}

	/* Ray origin inside bounding box */
	glm::vec3 coord;
	if (inside) 
	{
		coord = origin;
		return (TRUE);
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < NUMDIM; i++)
	{
		if (quadrant[i] != MIDDLE && dir[i] != 0.0f)
			maxT[i] = (candidatePlane[i] - origin[i]) / dir[i];
		else
			maxT[i] = -1.;
	}

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < NUMDIM; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.0f) 
		return (FALSE);

	for (i = 0; i < NUMDIM; i++)
	{
		if (whichPlane != i)
		{
			coord[i] = origin[i] + maxT[whichPlane] * dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return (FALSE);
		}
		else
		{
			coord[i] = candidatePlane[i];
		}
	}

	return (TRUE);				/* ray hits box */
}


inline bool rayTriangleIntersection
(
	const Ray& Ray,
	const glm::vec3& vert0,
	const glm::vec3& vert1,
	const glm::vec3& vert2,
	glm::vec3& intsPoint
)
{
	// Idea: Tomas Moeller and Ben Trumbore
	// in Fast, Minimum getRenderStorage Ray/Triangle Intersection 

	// Find vectors for two edges sharing vert0
	glm::vec3 edge1 = vert1 - vert0;
	glm::vec3 edge2 = vert2 - vert0;

	// Begin calculating determinant - also used to calculate U parameter
	glm::vec3 pvec = glm::cross(Ray.GetDirection(), edge2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = glm::dot(edge1, pvec);


	// *** Culling branch ***
	/*if( det < Math::Epsilon )return false;

	// Calculate distance from vert0 to ray origin
	vec3 tvec = Ray.GetOrigin() - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec );
	if (u < 0 || u > det ) return false;

	// Prepare to test V parameter
	vec3 qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = Ray.GetDirection().dot( qvec );
	if (v < 0 || u + v > det ) return false;

	// Calculate t, scale parameters, ray intersects triangle
	float t = edge2.dot( qvec ) / det;*/


	// *** Non-culling branch ***
	if (det > -Math::Epsilon && det < Math::Epsilon) return 0;
	float inv_det = 1.0f / det;

	// Calculate distance from vert0 to ray origin
	glm::vec3 tvec = Ray.GetOrigin() - vert0;

	// Calculate U parameter and test bounds
	float u = glm::dot(tvec, pvec) * inv_det;
	if (u < 0.0f || u > 1.0f) return 0;

	// Prepare to test V parameter
	glm::vec3 qvec = glm::cross(tvec, edge1);

	// Calculate V parameter and test bounds
	float v = glm::dot(Ray.GetDirection(), qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f) return 0;

	// Calculate t, ray intersects triangle
	float t = glm::dot(edge2, qvec) * inv_det;


	// Calculate intersection point and test ray length and direction
	intsPoint = Ray.GetOrigin() + Ray.GetDirection() * t;
	glm::vec3 vec = intsPoint - Ray.GetOrigin();
	if (glm::dot(vec, Ray.GetDirection()) < 0 || glm::length(vec) > glm::length(Ray.GetDirection())) return false;

	return true;
}

inline bool rayAABBIntersection
(
	const Ray& Ray,
	const BoundingBox& BoundingBox
)
{



	// SLAB based optimized ray/AABB intersection routine
	// Idea taken from http://ompf.org/ray/

	float l1 = (BoundingBox.getMin().x - Ray.GetOrigin().x) / Ray.GetDirection().x;
	float l2 = (BoundingBox.getMax().x - Ray.GetOrigin().x) / Ray.GetDirection().x;
	float lmin = glm::min(l1, l2);
	float lmax = glm::max(l1, l2);

	l1 = (BoundingBox.getMin().y - Ray.GetOrigin().y) / Ray.GetDirection().y;
	l2 = (BoundingBox.getMax().y - Ray.GetOrigin().y) / Ray.GetDirection().y;
	lmin = glm::max(minf(l1, l2), lmin);
	lmax = glm::min(maxf(l1, l2), lmax);

	l1 = (BoundingBox.getMin().z - Ray.GetOrigin().z) / Ray.GetDirection().z;
	l2 = (BoundingBox.getMax().z - Ray.GetOrigin().z) / Ray.GetDirection().z;
	lmin = glm::max(glm::min(l1, l2), lmin);
	lmax = glm::min(glm::max(l1, l2), lmax);

	if ((lmax >= 0.0f) & (lmax >= lmin))
	{
		// Consider length
		const glm::vec3 rayDest = Ray.GetOrigin() + Ray.GetDirection();
		glm::vec3 rayMins(glm::min(rayDest.x, Ray.GetOrigin().x), glm::min(rayDest.y, Ray.GetOrigin().y), glm::min(rayDest.z, Ray.GetOrigin().z));
		glm::vec3 rayMaxs(glm::max(rayDest.x, Ray.GetOrigin().x), glm::max(rayDest.y, Ray.GetOrigin().y), glm::max(rayDest.z, Ray.GetOrigin().z));
		return
			(rayMins.x < BoundingBox.getMax().x) && (rayMaxs.x > BoundingBox.getMin().x) &&
			(rayMins.y < BoundingBox.getMax().y) && (rayMaxs.y > BoundingBox.getMin().y) &&
			(rayMins.z < BoundingBox.getMax().z) && (rayMaxs.z > BoundingBox.getMin().z);
	}
	else
	{
		return false;
	}
}

inline float nearestDistToAABB
(
	const glm::vec3& pos,
	const BoundingBox& BoundingBox
)
{
	const glm::vec3 center = (BoundingBox.getMin() + BoundingBox.getMax()) * 0.5f;
	const glm::vec3 extent = (BoundingBox.getMax() - BoundingBox.getMin()) * 0.5f;

	glm::vec3 nearestVec;
	nearestVec.x = maxf(0, fabsf(pos.x - center.x) - extent.x);
	nearestVec.y = maxf(0, fabsf(pos.y - center.y) - extent.y);
	nearestVec.z = maxf(0, fabsf(pos.z - center.z) - extent.z);

	return glm::length(nearestVec);
}
