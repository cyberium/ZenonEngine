#pragma once;

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
// FORWARD END

class ZN_API Ray
{
public:
	Ray()
	{}

	Ray(glm::vec3 origin, glm::vec3 direction)
		: m_Origin(origin)
		, m_Direction(direction)
	{}

	virtual ~Ray()
	{}

	inline glm::vec3 GetOrigin() const
	{
		return m_Origin;
	}

	inline glm::vec3 GetDirection() const
	{
		return m_Direction;
	}

	inline glm::vec3 GetPointOnRay(float distance) const
	{
		return m_Origin + (m_Direction * distance);
	}

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;

	IMaterial* m_pMaterial;
};