#pragma once

#include <combaseapi.h>

class ZN_API Object
{
public:
	virtual bool operator==(const Object& rhs) const
	{
		return m_UUID == rhs.m_UUID;
	}

protected:
	Object()
	{
		::CoCreateGuid(&m_UUID);
	}
	virtual ~Object()
	{}

	GUID GetUUID() const
	{
		return m_UUID;
	}

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

private:
	GUID m_UUID;
};