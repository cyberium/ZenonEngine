#pragma once;

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
// FORWARD END

class OW_ENGINE_API Ray
{
public:
	Ray()
	{}

	Ray(vec3 origin, vec3 direction)
		: m_Origin(origin)
		, m_Direction(direction)
	{}

	virtual ~Ray()
	{}

	inline vec3 GetOrigin() const
	{
		return m_Origin;
	}

	inline vec3 GetDirection() const
	{
		return m_Direction;
	}

	inline vec3 GetPointOnRay(float distance) const
	{
		return m_Origin + (m_Direction * distance);
	}

private:
	vec3 m_Origin;
	vec3 m_Direction;

	IMaterial* m_pMaterial;
};