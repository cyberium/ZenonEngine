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

private:
	glm::vec3 m_Min;
	glm::vec3 m_Max;
	glm::vec3 m_Center;
	float m_Radius;
	bool m_IsCenterCalc;
};
