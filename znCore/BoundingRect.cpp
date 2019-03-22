#include "stdafx.h"

// General
#include "BoundingRect.h"

BoundingRect::BoundingRect() :
	m_Min(Math::MaxFloat, Math::MaxFloat),
	m_Max(Math::MinFloat, Math::MinFloat),
	m_IsCenterCalc(false)
{}

BoundingRect::BoundingRect(vec2 _min, vec2 _max) :
	m_Min(_min),
	m_Max(_max)
{
	calculateCenter();
}

void BoundingRect::set(glm::vec2 _min, glm::vec2 _max)
{
	m_Min = _min;
	m_Max = _max;

	calculateCenter();
}

void BoundingRect::calculate(const glm::vec2* _verts, uint32 _count)
{
	for (uint32 i = 0; i < _count; i++)
	{
        glm::vec2 v = _verts[i];

		if (v.x < m_Min.x) m_Min.x = v.x;
		if (v.y < m_Min.y) m_Min.y = v.y;

		if (v.x > m_Max.x) m_Max.x = v.x;
		if (v.y > m_Max.y) m_Max.y = v.y;
	}

	calculateCenter();
}

void BoundingRect::calculateCenter()
{
	m_Center = (m_Min + m_Max) * 0.5f;
	m_Radius = glm::length(m_Max - m_Center);
	m_IsCenterCalc = true;
}

void BoundingRect::clear()
{
	m_Min = vec2(0, 0);
	m_Max = vec2(0, 0);
	m_Center = vec2(0, 0);
	m_Radius = 0.0f;
	m_IsCenterCalc = false;
}

//

glm::vec2 BoundingRect::getCorner(uint32 index) const
{
	switch (index)
	{
	case 0:
		return vec2(m_Min.x, m_Min.y);
	case 1:
		return vec2(m_Max.x, m_Min.y);
	case 2:
		return vec2(m_Max.x, m_Max.y);
	case 3:
		return vec2(m_Min.x, m_Max.y);
	default:
		return vec2();
	}
}

void BoundingRect::transform(const mat4& m)
{
	float minA[2] = { m_Min.x, m_Min.y};
	float minB[2];
	float maxA[2] = { m_Max.x, m_Max.y};
	float maxB[2];

	for (uint32 i = 0; i < 2; ++i)
	{
		minB[i] = m[3][i];
		maxB[i] = m[3][i];

		for (uint32 j = 0; j < 2; ++j)
		{
			float x = minA[j] * m[j][i];
			float y = maxA[j] * m[j][i];
			minB[i] += minf(x, y);
			maxB[i] += maxf(x, y);
		}
	}

	m_Min = vec2(minB[0], minB[1]);
	m_Max = vec2(maxB[0], maxB[1]);

	assert1(m_IsCenterCalc);
	m_Center = m * vec4(m_Center, 0.0f, 0.0f);
}

bool BoundingRect::makeUnion(const BoundingRect& b)
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

		if (b.m_Max.x > m_Max.x) { changed = true; m_Max.x = b.m_Max.x; }
		if (b.m_Max.y > m_Max.y) { changed = true; m_Max.y = b.m_Max.y; }
	}

	calculateCenter();

	return changed;
}

bool BoundingRect::isPointInside(glm::vec2 _point) const
{
	if (_point.x < m_Min.x || _point.y < m_Min.y ||
		_point.x > m_Max.x || _point.y > m_Max.y)
	{
		return false;
	}

	return true;
}

BoundingRect BoundingRect::CreateBBoxInfinity()
{
	BoundingRect bbInf;
	bbInf.setMin(vec2(-100000.0f, -100000.0f));
	bbInf.setMax(vec2(100000.0f, 100000.0f));
	bbInf.calculateCenter();
	return bbInf;
}
