#include "stdafx.h"

// General
#include "Frustrum.h"

// Additional
#include "Plane.h"

Frustum::Frustum()
{
}

Frustum::Frustum(const Plane * Planes, uint32 PlanesCount)
{
	buildCustomFrustrum(Planes, PlanesCount);
}

Frustum::Frustum(const glm::mat4& viewMat, const glm::mat4& projMat)
{
	buildViewFrustum(viewMat, projMat);
}

void Frustum::buildCustomFrustrum(const Plane* _planes, uint32 _planesCount)
{
	m_Planes.resize(_planesCount);
	for (uint32 i = 0; i < _planesCount; i++)
	{
		m_Planes[i] = _planes[i];
	}
}

void Frustum::buildViewFrustum(const glm::mat4& transMat, float fov, float aspect, float nearPlane, float farPlane)
{
	float ymax = nearPlane * tanf(glm::radians(fov / 2));
	float xmax = ymax * aspect;

	buildViewFrustum(transMat, -xmax, xmax, -ymax, ymax, nearPlane, farPlane);
}

void Frustum::buildViewFrustum(const glm::mat4& transMat, float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	// Use intercept theorem to get params for far plane
	float left_f = left * farPlane / nearPlane;
	float right_f = right * farPlane / nearPlane;
	float bottom_f = bottom * farPlane / nearPlane;
	float top_f = top * farPlane / nearPlane;

	// Get points on near plane
	m_Corners[0] = glm::vec3(left, bottom, -nearPlane);
	m_Corners[1] = glm::vec3(right, bottom, -nearPlane);
	m_Corners[2] = glm::vec3(right, top, -nearPlane);
	m_Corners[3] = glm::vec3(left, top, -nearPlane);

	// Get points on far plane
	m_Corners[4] = glm::vec3(left_f, bottom_f, -farPlane);
	m_Corners[5] = glm::vec3(right_f, bottom_f, -farPlane);
	m_Corners[6] = glm::vec3(right_f, top_f, -farPlane);
	m_Corners[7] = glm::vec3(left_f, top_f, -farPlane);

	// Transform points to fit camera position and rotation
	m_Origin = transMat * glm::vec4(0, 0, 0, 0);
	for (uint32 i = 0; i < 8; ++i)
	{
		m_Corners[i] = transMat * glm::vec4(m_Corners[i], 0.0f);
	}

	// Build planes
	m_Planes.resize(6);
	m_Planes[0] = Plane(m_Origin, m_Corners[3], m_Corners[0]);		// Left
	m_Planes[1] = Plane(m_Origin, m_Corners[1], m_Corners[2]);		// Right
	m_Planes[2] = Plane(m_Origin, m_Corners[0], m_Corners[1]);		// Bottom
	m_Planes[3] = Plane(m_Origin, m_Corners[2], m_Corners[3]);		// Top
	m_Planes[4] = Plane(m_Corners[0], m_Corners[1], m_Corners[2]);	// Near
	m_Planes[5] = Plane(m_Corners[5], m_Corners[4], m_Corners[7]);	// Far
}

void Frustum::buildViewFrustum(const glm::mat4& viewMat, const glm::mat4& projMat)
{
	// This routine works with the OpenGL projection matrix
	// The view matrix is the inverse camera transformation matrix
	// Note: Frustum corners are not updated!

	const glm::mat4 m = projMat * viewMat;

	m_Planes.resize(6);
	m_Planes[0] = Plane(-(m[0][3] + m[0][0]), -(m[1][3] + m[1][0]),
		-(m[2][3] + m[2][0]), -(m[3][3] + m[3][0]));	// Left
	m_Planes[1] = Plane(-(m[0][3] - m[0][0]), -(m[1][3] - m[1][0]),
		-(m[2][3] - m[2][0]), -(m[3][3] - m[3][0]));	// Right
	m_Planes[2] = Plane(-(m[0][3] + m[0][1]), -(m[1][3] + m[1][1]),
		-(m[2][3] + m[2][1]), -(m[3][3] + m[3][1]));	// Bottom
	m_Planes[3] = Plane(-(m[0][3] - m[0][1]), -(m[1][3] - m[1][1]),
		-(m[2][3] - m[2][1]), -(m[3][3] - m[3][1]));	// Top
	m_Planes[4] = Plane(-(m[0][3] + m[0][2]), -(m[1][3] + m[1][2]),
		-(m[2][3] + m[2][2]), -(m[3][3] + m[3][2]));	// Near
	m_Planes[5] = Plane(-(m[0][3] - m[0][2]), -(m[1][3] - m[1][2]),
		-(m[2][3] - m[2][2]), -(m[3][3] - m[3][2]));	// Far

	m_Origin = glm::inverse(viewMat) * glm::vec4(0, 0, 0, 0);

	// Calculate corners
	glm::mat4 mm = glm::inverse(m);
	glm::vec4 corner = mm * glm::vec4(-1, -1, -1, 1);
	m_Corners[0] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
	corner = mm * glm::vec4(1, -1, -1, 1);
	m_Corners[1] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
	corner = mm * glm::vec4(1, 1, -1, 1);
	m_Corners[2] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
	corner = mm * glm::vec4(-1, 1, -1, 1);
	m_Corners[3] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
	corner = mm * glm::vec4(-1, -1, 1, 1);
	m_Corners[4] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
	corner = mm * glm::vec4(1, -1, 1, 1);
	m_Corners[5] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
	corner = mm * glm::vec4(1, 1, 1, 1);
	m_Corners[6] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
	corner = mm * glm::vec4(-1, 1, 1, 1);
	m_Corners[7] = glm::vec3(corner.x / corner.w, corner.y / corner.w, corner.z / corner.w);
}

void Frustum::buildBoxFrustum(const glm::mat4& transMat, float left, float right, float bottom, float top, float front, float back)
{
	// Get points on front plane
	m_Corners[0] = glm::vec3(left, bottom, front);
	m_Corners[1] = glm::vec3(right, bottom, front);
	m_Corners[2] = glm::vec3(right, top, front);
	m_Corners[3] = glm::vec3(left, top, front);

	// Get points on far plane
	m_Corners[4] = glm::vec3(left, bottom, back);
	m_Corners[5] = glm::vec3(right, bottom, back);
	m_Corners[6] = glm::vec3(right, top, back);
	m_Corners[7] = glm::vec3(left, top, back);

	// Transform points to fit camera position and rotation
	m_Origin = transMat * glm::vec4(0.0f);
	for (uint32 i = 0; i < 8; ++i)
		m_Corners[i] = transMat * glm::vec4(m_Corners[i], 0.0f);

	// Build planes
	m_Planes.resize(6);
	m_Planes[0] = Plane(m_Corners[0], m_Corners[3], m_Corners[7]);	// Left
	m_Planes[1] = Plane(m_Corners[2], m_Corners[1], m_Corners[6]);	// Right
	m_Planes[2] = Plane(m_Corners[0], m_Corners[4], m_Corners[5]);	// Bottom
	m_Planes[3] = Plane(m_Corners[3], m_Corners[2], m_Corners[6]);	// Top
	m_Planes[4] = Plane(m_Corners[0], m_Corners[1], m_Corners[2]);	// Front
	m_Planes[5] = Plane(m_Corners[4], m_Corners[7], m_Corners[6]);	// Back
}

void Frustum::buildBoxFrustum(Ray LT, Ray LB, Ray RT, Ray RB, float depth)
{
	// Get points on front plane
	m_Corners[0] = LB.GetOrigin();
	m_Corners[1] = RB.GetOrigin();
	m_Corners[2] = RT.GetOrigin();
	m_Corners[3] = LT.GetOrigin();

	// Get points on far plane
	m_Corners[4] = LB.GetOrigin() + LB.GetDirection() * depth;
	m_Corners[5] = RB.GetOrigin() + RB.GetDirection() * depth;
	m_Corners[6] = RT.GetOrigin() + RT.GetDirection() * depth;
	m_Corners[7] = LT.GetOrigin() + LT.GetDirection() * depth;

	m_Origin = glm::vec4(0);

	// Build planes
	m_Planes.resize(6);
	m_Planes[0] = Plane(m_Corners[0], m_Corners[3], m_Corners[7]);	// Left
	m_Planes[1] = Plane(m_Corners[2], m_Corners[1], m_Corners[6]);	// Right
	m_Planes[2] = Plane(m_Corners[0], m_Corners[4], m_Corners[5]);	// Bottom
	m_Planes[3] = Plane(m_Corners[3], m_Corners[2], m_Corners[6]);	// Top
	m_Planes[4] = Plane(m_Corners[0], m_Corners[1], m_Corners[2]);	// Front
	m_Planes[5] = Plane(m_Corners[4], m_Corners[7], m_Corners[6]);	// Back
}

//

bool Frustum::cullSphere(glm::vec3 pos, float rad) const
{
	for (size_t i = 0; i < m_Planes.size(); ++i)
		if (m_Planes[i].distToPoint(pos) > rad) 
			return true;

	return false;
}

bool Frustum::cullBox(const BoundingBox& b) const
{
	return cullBoxByPlanes(m_Planes.data(), m_Planes.size(), b);
}

bool Frustum::cullFrustum(const Frustum& frust) const
{
	for (size_t i = 0; i < m_Planes.size(); ++i)
	{
		bool allOut = true;

		for (uint32 j = 0; j < 8; ++j)
		{
			if (m_Planes[i].distToPoint(frust.m_Corners[j]) < 0)
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

bool Frustum::cullPoly(const glm::vec3* verts, uint32 nums) const
{
	return cullPolyByPlanes(m_Planes.data(), m_Planes.size(), verts, nums);
}

bool Frustum::cullPoly(const std::vector<glm::vec3>& PolyVertices) const
{
	return cullPolyByPlanes(m_Planes.data(), m_Planes.size(), PolyVertices);
}

//

void Frustum::calcAABB(glm::vec3& mins, glm::vec3& maxs) const
{
	mins.x = Math::MaxFloat; mins.y = Math::MaxFloat; mins.z = Math::MaxFloat;
	maxs.x = -Math::MaxFloat; maxs.y = -Math::MaxFloat; maxs.z = -Math::MaxFloat;

	for (uint32 i = 0; i < 8; ++i)
	{
		if (m_Corners[i].x < mins.x) mins.x = m_Corners[i].x;
		if (m_Corners[i].y < mins.y) mins.y = m_Corners[i].y;
		if (m_Corners[i].z < mins.z) mins.z = m_Corners[i].z;
		if (m_Corners[i].x > maxs.x) maxs.x = m_Corners[i].x;
		if (m_Corners[i].y > maxs.y) maxs.y = m_Corners[i].y;
		if (m_Corners[i].z > maxs.z) maxs.z = m_Corners[i].z;
	}
}