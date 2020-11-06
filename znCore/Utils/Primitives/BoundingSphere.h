#pragma once

class ZN_API BoundingSphere
{
public:
	BoundingSphere(const glm::vec3& Center = glm::vec3(0.0f), float Radius = Math::MaxFloat);

	const glm::vec3& GetCenter() const;
	float GetRadius() const;
	bool IsInfinite() const;

private:
	glm::vec3 m_Center;
	float m_Radius;
};