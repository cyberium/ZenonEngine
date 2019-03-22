#pragma once

// Include
#include "Math.h"

class BoundingBox
{
public:
	BoundingBox();
	explicit BoundingBox(vec3 _mix, vec3 _max);

	BoundingBox& operator=(const BoundingBox& _other)
	{
		m_Min = _other.m_Min;
		m_Max = _other.m_Max;
		m_Center = _other.m_Center;
		m_Radius = _other.m_Radius;
		m_IsCenterCalc = _other.m_IsCenterCalc;
		return *this;
	}

	void set(cvec3 _min, cvec3 _max, bool _needConvert = false);
	void calculate(const vec3* _verts, uint32 _count, bool _needConvert = false);
	void calculateCenter();
	void clear();

	//

	void setMin(cvec3 _min) { m_Min = _min; m_IsCenterCalc = false; }
	void setMinY(float value) { m_Min.y = value; m_IsCenterCalc = false; }
	cvec3 getMin() const { return m_Min; }

	void setMax(cvec3 _max) { m_Max = _max; m_IsCenterCalc = false; }
	void setMaxY(float value) { m_Max.y = value; m_IsCenterCalc = false; }
	cvec3 getMax() const { return m_Max; }

	cvec3 getCenter() const { assert1(m_IsCenterCalc); return m_Center; }
	float getRadius() const { assert1(m_IsCenterCalc); return m_Radius; }

	//

	vec3 getCorner(uint32 index) const;
	void transform(const mat4& m);
	bool makeUnion(const BoundingBox& b);
	bool isPointInside(cvec3 _point) const;

	// 

	static BoundingBox CreateBBoxInfinity();

private:
	vec3 m_Min;
	vec3 m_Max;
	vec3 m_Center;
	float m_Radius;
	bool m_IsCenterCalc;
};

typedef const BoundingBox& cbbox;


