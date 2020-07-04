#pragma once

#include "Plane.h"

class ZN_API Frustum
{
public:
	Frustum();
	Frustum(const Plane* Planes, uint32 PlanesCount);
	Frustum(const glm::mat4& viewMat, const glm::mat4& projMat);

	const std::vector<Plane> getPlanes() const { return m_Planes; }
	const glm::vec3& getOrigin() const { return m_Origin; }
	const glm::vec3& getCorner(uint32 index) const { return m_Corners[index]; }

	void buildCustomFrustrum(const Plane* _planes, uint32 _planesCount);
	void buildViewFrustum(const glm::mat4& transMat, float fov, float aspect, float nearPlane, float farPlane);
	void buildViewFrustum(const glm::mat4& transMat, float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void buildViewFrustum(const glm::mat4& viewMat, const glm::mat4& projMat);
	void buildBoxFrustum(const glm::mat4& transMat, float left, float right, float bottom, float top, float front, float back);
	void buildBoxFrustum(Ray LT, Ray LB, Ray RT, Ray RB, float depth);

	bool cullSphere(glm::vec3 pos, float rad) const;
	bool cullBox(const BoundingBox& b) const;
	bool cullFrustum(const Frustum& frust) const;
	bool cullPoly(const glm::vec3* verts, uint32 nums) const;
	bool cullPoly(const std::vector<glm::vec3>& PolyVertices) const;

	void calcAABB(glm::vec3& mins, glm::vec3& maxs) const;

private:
	std::vector<Plane> m_Planes;  // Planes of frustum
	glm::vec3  m_Origin;
	glm::vec3  m_Corners[8];  // Corner points
};