#pragma once

#include "Guid.h"

ZN_INTERFACE IManager;
ZN_INTERFACE IBaseManager;
ZN_INTERFACE IByteBuffer;
ZN_INTERFACE IXMLReader;
ZN_INTERFACE IXMLWriter;

ZN_INTERFACE ZN_API IObject
{
	virtual ~IObject() {}

	virtual Guid GetGUID() const = 0;
	virtual std::string GetName() const = 0;
	virtual void SetName(const std::string& Name) = 0;

	inline bool IsType(ObjectType Type) const { return GetType() == Type; }
	inline bool IsClass(ObjectClass Class) const { return GetClass() == Class; }
	inline ObjectClass GetType() const { return GetGUID().GetObjectClass(); }
	inline ObjectClass GetClass() const { return GetGUID().GetObjectClass(); }
	inline ObjectCounterType GetID() const { return GetGUID().GetCounter(); }
	inline bool operator==(const IObject& rhs) const { return GetGUID() == rhs.GetGUID(); }
	inline bool operator!=(const IObject& rhs) const { return GetGUID() != rhs.GetGUID(); }
};


#define OBJECT_IMPLEMENT_BASE \
	Guid GetGUID() const override { return Object::GetGUID(); }; \
	std::string GetName() const override { return Object::GetName(); }; \
	void SetName(const std::string& Name) override { Object::SetName(Name); };


ZN_INTERFACE ZN_API IObjectPrivate
{
	virtual ~IObjectPrivate() {}

	virtual void SetGUID(const Guid& NewGuid) = 0;
};


ZN_INTERFACE ZN_API	IObjectLoadSave
{
	virtual ~IObjectLoadSave() {}

	virtual void CopyTo(std::shared_ptr<IObject> Destination) const = 0;
	virtual void Load(const std::shared_ptr<IByteBuffer>& Buffer) = 0;
	virtual void Save(const std::shared_ptr<IByteBuffer>& Buffer) const = 0;
	virtual void Load(const std::shared_ptr<IXMLReader>& Reader) = 0;
	virtual void Save(const std::shared_ptr<IXMLWriter>& Writer) const = 0;
};
