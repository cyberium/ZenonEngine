#pragma once

// Include
#include "Math.h"

class BoundingRect
{
public:
	BoundingRect();
	explicit BoundingRect(glm::vec2 _mix, glm::vec2 _max);

	BoundingRect& operator=(const BoundingRect& _other)
	{
		m_Min = _other.m_Min;
		m_Max = _other.m_Max;
		m_Center = _other.m_Center;
		m_Radius = _other.m_Radius;
		m_IsCenterCalc = _other.m_IsCenterCalc;
		return *this;
	}

	void set(glm::vec2 _min, glm::vec2 _max);
	void calculate(const glm::vec2* _verts, uint32 _count);
	void calculateCenter();
	void clear();

	//

	void setMin(glm::vec2 _min) { m_Min = _min; m_IsCenterCalc = false; }
	glm::vec2 getMin() const { return m_Min; }

	void setMax(glm::vec2 _max) { m_Max = _max; m_IsCenterCalc = false; }
	glm::vec2 getMax() const { return m_Max; }

	glm::vec2 getCenter() const { assert1(m_IsCenterCalc); return m_Center; }
	float getRadius() const { assert1(m_IsCenterCalc); return m_Radius; }

	//

    glm::vec2 getCorner(uint32 index) const;
	void transform(const mat4& m);
	bool makeUnion(const BoundingRect& b);
	bool isPointInside(glm::vec2 _point) const;

	// 

	static BoundingRect CreateBBoxInfinity();

private:
	vec2 m_Min;
	vec2 m_Max;
	vec2 m_Center;
	float m_Radius;
	bool m_IsCenterCalc;
};

typedef const BoundingRect& cbrect;


