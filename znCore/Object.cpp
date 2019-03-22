#include "stdafx.h"

// General
#include "Object.h"

// Additoonal
#include <combaseapi.h>

Object::Object()
{
	::CoCreateGuid(&m_UUID);
}

Object::Object(ConstructorType type)
{}

Object::~Object()
{}

bool Object::operator==(const Object& rhs) const
{
	return m_UUID == rhs.m_UUID;
}

GUID Object::GetUUID() const
{
	return m_UUID;
}
