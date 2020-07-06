#pragma once

#include "Guid.h"

ZN_INTERFACE IManager;
ZN_INTERFACE IByteBuffer;

ZN_INTERFACE ZN_API IObject
{
	virtual ~IObject() {}

	// Properties
	virtual Guid GetGUID() const = 0;
	virtual ObjectCounterType GetID() const = 0;
	virtual std::string GetName() const = 0;
	
	virtual ObjectType GetType() const = 0;
	virtual std::string GetTypeName() const = 0;
	virtual bool IsType(ObjectType Type) const = 0;

	virtual ObjectClass GetClass() const = 0;
	virtual std::string GetClassName() const = 0;
	virtual bool IsClass(ObjectClass Class) const = 0;

	// Methods
	virtual void Copy(std::shared_ptr<IObject> Destination) const = 0;
};

ZN_INTERFACE ZN_API IObjectInternal
{
	virtual ~IObjectInternal() {}

	virtual void SetName(const std::string& Name) = 0;
	virtual void SetTypeName(const std::string& Name) = 0;
	virtual void SetClassName(const std::string& Name) = 0;
};

ZN_INTERFACE ZN_API IObjectPrivate
{
	virtual ~IObjectPrivate() {}

	virtual void SetGUID(const Guid& NewGuid) = 0;
};

ZN_INTERFACE ZN_API	IObjectLoadSave
{
	virtual ~IObjectLoadSave() {}

	virtual void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) = 0;
	virtual void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) = 0;
};

class ZN_API Object
	: public IObject
	, public IObjectInternal
	, public IObjectPrivate
{
public:
	bool operator==(const Object& rhs) const
	{
		return m_Guid == rhs.m_Guid;
	}
	bool operator!=(const Object& rhs) const
	{
		return m_Guid != rhs.m_Guid;
	}

	//
	// IObject
	//
	Guid GetGUID() const override
	{
		return m_Guid;
	}
	ObjectCounterType GetID() const override
	{
		return GetGUID().GetCounter();
	}
	std::string GetName() const override
	{
		//_ASSERT(m_ClassName.empty() == false);
		return m_Name;
	}


	// Type
	ObjectType GetType() const override
	{
		return m_Guid.GetObjectType();
	}
	std::string GetTypeName() const override
	{
		//_ASSERT(m_ClassName.empty() == false);
		return m_TypeName;
	}
	bool IsType(ObjectType Type) const override
	{
		return GetType() == Type;
	}


	// Class
	ObjectClass GetClass() const override
	{
		return m_Guid.GetObjectClass();
	}
	std::string GetClassName() const override
 	{
		_ASSERT(m_ClassName.empty() == false);
		return m_ClassName;
	}
	bool IsClass(ObjectClass Class) const override
	{
		return GetClass() == Class;
	}


	void Copy(std::shared_ptr<IObject> Destination) const
	{
		auto object = std::static_pointer_cast<Object>(Destination);

		if (GetGUID().GetObjectClass() != object->GetGUID().GetObjectClass() || GetGUID().GetObjectType() != object->GetGUID().GetObjectType())
			throw std::exception(("Unable to copy object with different type and class. Source: " + GetGUID().Str() + ", Destination: " + object->GetGUID().Str()).c_str());

		object->m_Name = m_Name;
		object->m_ClassName = m_ClassName;
		object->m_TypeName = m_TypeName;
	}
	

	//
	// IObjectInternal
	//
	void SetName(const std::string& Name) override
	{
		m_Name = Name;
	}
	void SetClassName(const std::string& Name)  override
	{
		m_ClassName = Name;
	}
	void SetTypeName(const std::string& Name) override
	{
		m_TypeName = Name;
	}

	
private:
	//
	// IObjectPrivate
	//
	void SetGUID(const Guid& NewGuid) override 
	{
		if (GetGUID().IsEmpty() == false)
			throw std::exception(("Object " + GetGUID().Str() + " already has Guid.").c_str());

		if (NewGuid.IsEmpty())
			throw std::exception("Cannot assign empty GUID for object.");

		if (NewGuid.GetCounter() == 0)
			throw std::exception(("Guid " + NewGuid.Str() + " not fully initialized.").c_str());

		m_Guid = Guid(NewGuid.GetRawValue());

		if (m_Name.empty())
			m_Name = GetClassName() + "_id" + std::to_string(GetGUID().GetCounter());

		if (m_ClassName.empty())
			m_ClassName = "class" + std::to_string(GetClass());

		if (m_TypeName.empty())
			m_TypeName = "type" + std::to_string(GetType());
	}


protected:
	Object()
		: m_Guid(0ull)
	{
		m_Name = "";
	}
	Object(ObjectType Factory, ObjectClass Class)
		: m_Guid(0ull)
	{
		m_Name = "";
	}
	virtual ~Object()
	{}

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

private:
	Guid m_Guid;
	std::string m_Name;
	std::string m_ClassName;
	std::string m_TypeName;
};
