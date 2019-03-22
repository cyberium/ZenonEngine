#pragma once

#include "Plane.h"

class Frustum
{
public:

	Frustum& operator=(const Frustum& _other)
	{
		m_PlanesCount = _other.m_PlanesCount;
		for (uint32 i = 0; i < this->m_PlanesCount; i++)
		{
			m_Planes[i] = _other.m_Planes[i];
		}
	}

	const Plane* getPlanes() const { return &m_Planes[0]; }
	const uint32 getPlanesCnt() const { return m_PlanesCount; }
	const vec3& getOrigin() const { return m_Origin; }
	const vec3& getCorner(uint32 index) const { return m_Corners[index]; }

	void buildCustomFrustrum(const Plane* _planes, uint32 _planesCount);
	void buildViewFrustum(const mat4 &transMat, float fov, float aspect, float nearPlane, float farPlane);
	void buildViewFrustum(const mat4 &transMat, float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void buildViewFrustum(const mat4 &viewMat, const mat4 &projMat);
	void buildBoxFrustum(const mat4 &transMat, float left, float right, float bottom, float top, float front, float back);

	bool cullSphere(vec3 pos, float rad) const;
	bool cullBox(const BoundingBox& b) const;
	bool cullFrustum(const Frustum& frust) const;
	bool cullPoly(const vec3* verts, uint32 nums) const;

	void calcAABB(vec3 &mins, vec3 &maxs) const;



private:
	Plane  m_Planes[20];  // Planes of frustum
	uint8  m_PlanesCount;

	vec3  m_Origin;
	vec3  m_Corners[8];  // Corner points
};