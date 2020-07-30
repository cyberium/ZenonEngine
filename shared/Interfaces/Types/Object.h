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

	// Properties
	virtual Guid GetGUID() const = 0;
	virtual std::string GetName() const = 0;
	virtual void SetName(const std::string& Name) = 0;

	// Methods
	//virtual void Copy(std::shared_ptr<IObject> Destination) const = 0;

	inline bool IsType(ObjectType Type) const { return GetType() == Type; }
	inline bool IsClass(ObjectClass Class) const { return GetClass() == Class; }
	inline ObjectClass GetType() const { return GetGUID().GetObjectClass(); }
	inline ObjectClass GetClass() const { return GetGUID().GetObjectClass(); }
	inline ObjectCounterType GetID() const { return GetGUID().GetCounter(); }
};

ZN_INTERFACE ZN_API IObjectPrivate
{
	virtual ~IObjectPrivate() {}

	virtual void SetGUID(const IBaseManager* BaseManager,const Guid& NewGuid) = 0;
};

ZN_INTERFACE ZN_API	IObjectLoadSave
{
	virtual ~IObjectLoadSave() {}

	virtual void Load(const std::shared_ptr<IByteBuffer>& Buffer) = 0;
	virtual void Save(const std::shared_ptr<IByteBuffer>& Buffer) const = 0;
	virtual void Load(const std::shared_ptr<IXMLReader>& Reader) = 0;
	virtual void Save(const std::shared_ptr<IXMLWriter>& Writer) const = 0;
};
