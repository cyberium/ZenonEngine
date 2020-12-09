#include "stdafx.h"

// General
#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	clear();
}

BoundingBox::BoundingBox(const glm::vec3& Min, const glm::vec3& Max)
{
	set(Min, Max);
}

BoundingBox& BoundingBox::operator=(const BoundingBox& Other)
{
	m_Min = Other.m_Min;
	m_Max = Other.m_Max;
	m_Center = Other.m_Center;
	m_Radius = Other.m_Radius;
	return *this;
}

void BoundingBox::set(const glm::vec3& Min, const glm::vec3& Max)
{
	m_Min = Min;
	m_Max = Max;
	CalculateCenter();
}

void BoundingBox::calculate(const glm::vec3* _verts, uint32 _count)
{
	for (uint32 i = 0; i < _count; i++)
	{
		const glm::vec3 v = _verts[i];

		if (v.x < m_Min.x) m_Min.x = v.x;
		if (v.y < m_Min.y) m_Min.y = v.y;
		if (v.z < m_Min.z) m_Min.z = v.z;

		if (v.x > m_Max.x) m_Max.x = v.x;
		if (v.y > m_Max.y) m_Max.y = v.y;
		if (v.z > m_Max.z) m_Max.z = v.z;
	}

	CalculateCenter();
}

void BoundingBox::clear()
{
	m_Min = glm::vec3(Math::MinFloat);
	m_Max = glm::vec3(Math::MaxFloat);
	ResetCenter();
}

bool BoundingBox::IsInfinite() const
{
	return (m_Min.x == Math::MinFloat) || (m_Min.y == Math::MinFloat) || (m_Min.z == Math::MinFloat) ||
		   (m_Max.x == Math::MaxFloat) || (m_Max.y == Math::MaxFloat) || (m_Max.z == Math::MaxFloat) ||
		   (m_Min.x == Math::MaxFloat) || (m_Min.y == Math::MaxFloat) || (m_Min.z == Math::MaxFloat) ||
		   (m_Max.x == Math::MinFloat) || (m_Max.y == Math::MinFloat) || (m_Max.z == Math::MinFloat);
}

void BoundingBox::setMin(const glm::vec3& _min)
{
	m_Min = _min;
	CalculateCenter();
}

const glm::vec3& BoundingBox::getMin() const
{
	return m_Min;
}

void BoundingBox::setMax(const glm::vec3& _max)
{
	m_Max = _max; 
	CalculateCenter();
}

const glm::vec3& BoundingBox::getMax() const
{
	return m_Max;
}

const glm::vec3& BoundingBox::getCenter() const
{
	_ASSERT(IsCenterCalculated());
	return m_Center;
}

float BoundingBox::getRadius() const
{
	_ASSERT(IsCenterCalculated()); 
	return m_Radius;
}

//

glm::vec3 BoundingBox::getCorner(uint32 index) const
{
	switch (index)
	{
	case 0:
		return glm::vec3(m_Min.x, m_Min.y, m_Max.z);
	case 1:
		return glm::vec3(m_Max.x, m_Min.y, m_Max.z);
	case 2:
		return glm::vec3(m_Max.x, m_Max.y, m_Max.z);
	case 3:
		return glm::vec3(m_Min.x, m_Max.y, m_Max.z);
	case 4:
		return glm::vec3(m_Min.x, m_Min.y, m_Min.z);
	case 5:
		return glm::vec3(m_Max.x, m_Min.y, m_Min.z);
	case 6:
		return glm::vec3(m_Max.x, m_Max.y, m_Min.z);
	case 7:
		return glm::vec3(m_Min.x, m_Max.y, m_Min.z);
	default:
		return glm::vec3();
	}
}

void BoundingBox::transform(const glm::mat4& m)
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
			minB[i] += glm::min(x, y);
			maxB[i] += glm::max(x, y);
		}
	}

	m_Min = glm::vec3(minB[0], minB[1], minB[2]);
	m_Max = glm::vec3(maxB[0], maxB[1], maxB[2]);

	CalculateCenter();
}

bool BoundingBox::makeUnion(const BoundingBox& b)
{
	if (IsInfinite())
	{
		m_Min = glm::vec3(Math::MaxFloat);
		m_Max = glm::vec3(Math::MinFloat);
		ResetCenter();
	}

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

	CalculateCenter();

	return changed;
}

bool BoundingBox::isPointInside(const glm::vec3& Point) const
{
	if (Point.x < m_Min.x || Point.y < m_Min.y || Point.z < m_Min.z ||
		Point.x > m_Max.x || Point.y > m_Max.y || Point.z > m_Max.z)
	{
		return false;
	}

	return true;
}


void BoundingBox::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	ByteBuffer->read(&m_Min);
	ByteBuffer->read(&m_Max);
	CalculateCenter();
}

bool BoundingBox::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	if (IsInfinite())
		return false;

	ByteBuffer->write(&m_Min);
	ByteBuffer->write(&m_Max);
	return true;
}

void BoundingBox::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	if (false == Reader->IsAttributeExists("Min") || false == Reader->IsAttributeExists("Max"))
		return;

	m_Min = Reader->GetVec3Attribute("Min");
	m_Max = Reader->GetVec3Attribute("Max");
	CalculateCenter();
}

bool BoundingBox::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	if (IsInfinite())
		return false;

	Writer->SetVec3Attribute(m_Min, "Min");
	Writer->SetVec3Attribute(m_Max, "Max");
	return true;
}



//
// Private
//
bool BoundingBox::IsCenterCalculated() const
{
	return (m_Center.x != Math::MaxFloat) && (m_Center.y != Math::MaxFloat) && (m_Center.z != Math::MaxFloat) && (m_Radius != Math::MaxFloat);
}

void BoundingBox::CalculateCenter()
{
	if (IsInfinite())
		return;
	m_Center = (m_Min + m_Max) * 0.5f;
	m_Radius = glm::length(m_Max - m_Center);
}

void BoundingBox::ResetCenter()
{
	m_Center = glm::vec3(Math::MaxFloat);
	m_Radius = Math::MaxFloat;
}
