#pragma once

#include <combaseapi.h>

typedef ZN_API int64 ObjectFactoryKey;
typedef ZN_API int64 ObjectClassKey;

class ZN_API Object
{
public:
	virtual bool operator==(const Object& rhs) const
	{
		return m_UUID == rhs.m_UUID;
	}

	GUID GetUUID() const
	{
		return m_UUID;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
	std::string GetName() const
	{
		return m_Name;
	}

	void SetFactoryKey(ObjectFactoryKey Type)
	{
		m_FactoryKey = Type;
	}
	ObjectFactoryKey GetFactoryKey() const
	{
		return m_FactoryKey;
	}

	void SetClassName(const std::string& ClassName)
	{
		m_ClassName = ClassName;
		m_ClassKey = std::hash<std::string>{}(m_ClassName);
	}
	std::string GetClassName() const
	{
		return m_ClassName;
	}
	ObjectClassKey GetClassKey() const
	{
		return m_ClassKey;
	}

	bool Is(ObjectFactoryKey ObjectType) const
	{
		return m_FactoryKey == ObjectType;
	}

protected:
	Object()
		: m_ClassKey(0)
	{
		::CoCreateGuid(&m_UUID);
		
		SetName("Object");
		SetClassName("Object");
		SetFactoryKey(0);
	}
	virtual ~Object()
	{}

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

private:
	GUID m_UUID;
	
	std::string m_Name;
	std::string m_ClassName;
	ObjectClassKey m_ClassKey;
	ObjectFactoryKey m_FactoryKey;
};


ZN_INTERFACE ZN_API IObjectFactory
{
	virtual ~IObjectFactory() {}

	virtual std::shared_ptr<Object> CreateObject(ObjectFactoryKey ObjectFactoryKey, ObjectClassKey ObjectClassKey) = 0;
};
