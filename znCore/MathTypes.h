#pragma once

#include "MathCommon.h"
#include "Matrix.h"

// -------------------------------------------------------------------------------------------------
// Intersection
// -------------------------------------------------------------------------------------------------

inline bool rayTriangleIntersection
(
	const glm::vec3& rayOrig, 
	const glm::vec3& rayDir,
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
	glm::vec3 pvec = glm::cross(rayDir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = glm::dot(edge1, pvec);


	// *** Culling branch ***
	/*if( det < Math::Epsilon )return false;

	// Calculate distance from vert0 to ray origin
	vec3 tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = tvec.dot( pvec );
	if (u < 0 || u > det ) return false;

	// Prepare to test V parameter
	vec3 qvec = tvec.cross( edge1 );

	// Calculate V parameter and test bounds
	float v = rayDir.dot( qvec );
	if (v < 0 || u + v > det ) return false;

	// Calculate t, scale parameters, ray intersects triangle
	float t = edge2.dot( qvec ) / det;*/


	// *** Non-culling branch ***
	if (det > -Math::Epsilon && det < Math::Epsilon) return 0;
	float inv_det = 1.0f / det;

	// Calculate distance from vert0 to ray origin
	glm::vec3 tvec = rayOrig - vert0;

	// Calculate U parameter and test bounds
	float u = glm::dot(tvec, pvec) * inv_det;
	if (u < 0.0f || u > 1.0f) return 0;

	// Prepare to test V parameter
	glm::vec3 qvec = glm::cross(tvec, edge1);

	// Calculate V parameter and test bounds
	float v = glm::dot(rayDir, qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f) return 0;

	// Calculate t, ray intersects triangle
	float t = glm::dot(edge2, qvec) * inv_det;


	// Calculate intersection point and test ray length and direction
	intsPoint = rayOrig + rayDir * t;
	glm::vec3 vec = intsPoint - rayOrig;
	if (glm::dot(vec, rayDir) < 0 || glm::length(vec) > glm::length(rayDir)) return false;

	return true;
}




inline bool rayAABBIntersection
(
	const glm::vec3& rayOrig, 
	const glm::vec3& rayDir,				
	const glm::vec3& mins, 
	const glm::vec3& maxs
)
{
	// SLAB based optimized ray/AABB intersection routine
	// Idea taken from http://ompf.org/ray/

	float l1 = (mins.x - rayOrig.x) / rayDir.x;
	float l2 = (maxs.x - rayOrig.x) / rayDir.x;
	float lmin = minf(l1, l2);
	float lmax = maxf(l1, l2);

	l1 = (mins.y - rayOrig.y) / rayDir.y;
	l2 = (maxs.y - rayOrig.y) / rayDir.y;
	lmin = maxf(minf(l1, l2), lmin);
	lmax = minf(maxf(l1, l2), lmax);

	l1 = (mins.z - rayOrig.z) / rayDir.z;
	l2 = (maxs.z - rayOrig.z) / rayDir.z;
	lmin = maxf(minf(l1, l2), lmin);
	lmax = minf(maxf(l1, l2), lmax);

	if ((lmax >= 0.0f) & (lmax >= lmin))
	{
		// Consider length
		const glm::vec3 rayDest = rayOrig + rayDir;
		glm::vec3 rayMins(minf(rayDest.x, rayOrig.x), minf(rayDest.y, rayOrig.y), minf(rayDest.z, rayOrig.z));
		glm::vec3 rayMaxs(maxf(rayDest.x, rayOrig.x), maxf(rayDest.y, rayOrig.y), maxf(rayDest.z, rayOrig.z));
		return
			(rayMins.x < maxs.x) && (rayMaxs.x > mins.x) &&
			(rayMins.y < maxs.y) && (rayMaxs.y > mins.y) &&
			(rayMins.z < maxs.z) && (rayMaxs.z > mins.z);
	}
	else
	{
		return false;
	}
}


inline float nearestDistToAABB
(
	const glm::vec3& pos, 
	const glm::vec3& mins, 
	const glm::vec3& maxs
)
{
	const glm::vec3 center = (mins + maxs) * 0.5f;
	const glm::vec3 extent = (maxs - mins) * 0.5f;

	glm::vec3 nearestVec;
	nearestVec.x = maxf(0, fabsf(pos.x - center.x) - extent.x);
	nearestVec.y = maxf(0, fabsf(pos.y - center.y) - extent.y);
	nearestVec.z = maxf(0, fabsf(pos.z - center.z) - extent.z);

	return glm::length(nearestVec);
}

inline glm::vec3 screenToWord
(
	const glm::vec2& _mousePos, 
	const glm::vec2& windowSize, 
	const glm::mat4& projection_matrix, 
	const glm::mat4& view_matrix
)
{
	float x = (2.0f * _mousePos.x) / windowSize.x - 1.0f;
	float y = 1.0f - (2.0f * _mousePos.y) / windowSize.y;

	glm::vec2 ray_nds = glm::vec2(x, y);

	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	glm::vec3 ray_wor = glm::inverse(view_matrix) * ray_eye;
	ray_wor = glm::normalize(ray_wor);

	return ray_wor;
}
