#pragma once

class BoundingSphere
{
public:
	BoundingSphere(cvec3 center = vec3(0), float radius = 0);

	cvec3 GetCenter() const;
	float GetRadius() const;
	float GetInvRadiusSqr() const;

	// Valid if the radius is > 0.
	bool IsValid() const;

	/**
	 * Enlarge this bounding sphere by adding another bounding sphere to it.
	 */
	void Enlarge(const BoundingSphere& other);

private:
	vec3	m_Center;
	float	m_Radius;
	float	m_InvRadiusSqr;    // 1 / ( radius^2)

};