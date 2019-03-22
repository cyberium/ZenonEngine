#include "stdafx.h"

// General
#include "BoundingBox.h"

BoundingBox::BoundingBox() :
	m_Min(vec3(Math::MaxFloat, Math::MaxFloat, Math::MaxFloat)),
	m_Max(vec3(Math::MinFloat, Math::MinFloat, Math::MinFloat)),
	m_IsCenterCalc(false)
{}

BoundingBox::BoundingBox(vec3 _min, vec3 _max) :
	m_Min(_min),
	m_Max(_max)
{
	calculateCenter();
}

void BoundingBox::set(cvec3 _min, cvec3 _max, bool _needConvert)
{
	m_Min = _min;
	m_Max = _max;

	// Fix bounding box
	if (_needConvert)
	{
		m_Min = Fix_XZmY(m_Min);
		m_Max = Fix_XZmY(m_Max);
		std::swap(m_Min.z, m_Max.z);
	}

	//assert1(min.x < max.x && min.y < max.y && min.z < max.z);

	calculateCenter();
}

void BoundingBox::calculate(const vec3* _verts, uint32 _count, bool _needConvert)
{
	for (uint32 i = 0; i < _count; i++)
	{
		vec3 v = (_needConvert) ? (Fix_XZmY(_verts[i])) : _verts[i];

		if (v.x < m_Min.x) m_Min.x = v.x;
		if (v.y < m_Min.y) m_Min.y = v.y;
		if (v.z < m_Min.z) m_Min.z = v.z;

		if (v.x > m_Max.x) m_Max.x = v.x;
		if (v.y > m_Max.y) m_Max.y = v.y;
		if (v.z > m_Max.z) m_Max.z = v.z;
	}

	calculateCenter();
}

void BoundingBox::calculateCenter()
{
	m_Center = (m_Min + m_Max) * 0.5f;
	m_Radius = glm::length(m_Max - m_Center);
	m_IsCenterCalc = true;
}

void BoundingBox::clear()
{
	m_Min = vec3(0, 0, 0);
	m_Max = vec3(0, 0, 0);
	m_Center = vec3(0, 0, 0);
	m_Radius = 0.0f;
	m_IsCenterCalc = false;
}

//

vec3 BoundingBox::getCorner(uint32 index) const
{
	switch (index)
	{
	case 0:
		return vec3(m_Min.x, m_Min.y, m_Max.z);
	case 1:
		return vec3(m_Max.x, m_Min.y, m_Max.z);
	case 2:
		return vec3(m_Max.x, m_Max.y, m_Max.z);
	case 3:
		return vec3(m_Min.x, m_Max.y, m_Max.z);
	case 4:
		return vec3(m_Min.x, m_Min.y, m_Min.z);
	case 5:
		return vec3(m_Max.x, m_Min.y, m_Min.z);
	case 6:
		return vec3(m_Max.x, m_Max.y, m_Min.z);
	case 7:
		return vec3(m_Min.x, m_Max.y, m_Min.z);
	default:
		return vec3();
	}
}

void BoundingBox::transform(const mat4& m)
{
	// Efficient algorithm for transforming an AABB, taken from Graphics Gems

	float minA[3] = { m_Min.x, m_Min.y, m_Min.z };
	float minB[3];
	float maxA[3] = { m_Max.x, m_Max.y, m_Max.z };
	float maxB[3];

	for (uint32 i = 0; i < 3; ++i)
	{
		minB[i] = m[3][i];
		maxB[i] = m[3][i];

		for (uint32 j = 0; j < 3; ++j)
		{
			float x = minA[j] * m[j][i];
			float y = maxA[j] * m[j][i];
			minB[i] += minf(x, y);
			maxB[i] += maxf(x, y);
		}
	}

	m_Min = vec3(minB[0], minB[1], minB[2]);
	m_Max = vec3(maxB[0], maxB[1], maxB[2]);

	assert1(m_IsCenterCalc);
	m_Center = m * vec4(m_Center, 0);
}

bool BoundingBox::makeUnion(const BoundingBox& b)
{
	bool changed = false;

	// Ignore zero-size boxes
	if (m_Min == m_Max)
	{
		changed = true;
		m_Min = b.m_Min;
		m_Max = b.m_Max;
	}
	else if (b.m_Min != b.m_Max)
	{
		if (b.m_Min.x < m_Min.x) { changed = true; m_Min.x = b.m_Min.x; }
		if (b.m_Min.y < m_Min.y) { changed = true; m_Min.y = b.m_Min.y; }
		if (b.m_Min.z < m_Min.z) { changed = true; m_Min.z = b.m_Min.z; }

		if (b.m_Max.x > m_Max.x) { changed = true; m_Max.x = b.m_Max.x; }
		if (b.m_Max.y > m_Max.y) { changed = true; m_Max.y = b.m_Max.y; }
		if (b.m_Max.z > m_Max.z) { changed = true; m_Max.z = b.m_Max.z; }
	}

	calculateCenter();

	return changed;
}

bool BoundingBox::isPointInside(cvec3 _point) const
{
	if (_point.x < m_Min.x || _point.y < m_Min.y ||	_point.z < m_Min.z ||
		_point.x > m_Max.x || _point.y > m_Max.y ||	_point.z > m_Max.z)
	{
		return false;
	}

	return true;
}

BoundingBox BoundingBox::CreateBBoxInfinity()
{
	BoundingBox bbInf;
	bbInf.setMin(vec3(-100000.0f, -100000.0f, -100000.0f));
	bbInf.setMax(vec3(100000.0f, 100000.0f, 100000.0f));
	bbInf.calculateCenter();
	return bbInf;
}
