#include "stdafx.h"

// General
#include "ObjectsFactory.h"

// Additional
#include "Files/ByteBuffer.h"

CObjectsFactory::CObjectsFactory(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CObjectsFactory::~CObjectsFactory()
{
}

//
// IObjectsFactory
//
std::shared_ptr<IObjectClassFactory> CObjectsFactory::GetClassFactory(ObjectType ObjectFactoryKey) const
{
	const auto& it = m_ClassFactories.find(ObjectFactoryKey);
	if (it == m_ClassFactories.end())
		throw CException("ObjectsFactory : ClassFactory '%d' not found.", ObjectFactoryKey);
	return it->second;
}

void CObjectsFactory::AddClassFactory(std::shared_ptr<IObjectClassFactory> Creator)
{
	auto creatorType = Creator->GetType();
	const auto& it = m_ClassFactories.find(creatorType);
	if (it != m_ClassFactories.end())
		throw CException("ObjectsFactory: ClassFactory '%d' already exists!", creatorType);
	m_ClassFactories.insert(std::make_pair(creatorType, Creator));
}

void CObjectsFactory::RemoveClassFactory(std::shared_ptr<IObjectClassFactory> Creator)
{
	auto creatorType = Creator->GetType();
	const auto& it = m_ClassFactories.find(creatorType);
	if (it == m_ClassFactories.end())
		throw CException("ObjectsFactory: Unable to remove ClassFactory '%d' because not found in ObjectFactory.", creatorType);
	m_ClassFactories.erase(it);
}

std::shared_ptr<IObject> CObjectsFactory::CreateObject(ObjectType ObjectFactoryKey, ObjectClass ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs)
{
	Guid objectUUID(ObjectFactoryKey, ObjectClassKey, 0u);

	try
	{
		return GetClassFactory(ObjectFactoryKey)->CreateObject(ObjectClassKey, ObjectCreationArgs);
	}
	catch (const CException& e)
	{
		Log::Error("ObjectsFactory: Unable to create object [%s]", objectUUID.CStr());
		return nullptr;
	}
}

const uint32 cObjectBinarySignatureBegin = 0x01234567;
const uint32 cObjectBinarySignatureEnd   = 0x76543210;

std::shared_ptr<IObject> CObjectsFactory::LoadObject(std::shared_ptr<IByteBuffer> Bytes)
{
	uint32 storedSignature = 0;
	Bytes->read(&storedSignature);
	if (storedSignature != cObjectBinarySignatureBegin)
		throw CException("ObjectsFactory: Bytes is not ZenonObject.");

	uint64 guidUInt = 0;
	Bytes->read(&guidUInt);
	Guid storedGuid(guidUInt);
	ObjectType storedFactoryKey = storedGuid.GetObjectType();
	ObjectClass storedClassType = storedGuid.GetObjectClass();

	std::string storedName;
	Bytes->readString(&storedName);
	
	/*try
	{
		return GetClassFactory(storedFactoryKey)->CreateObject(ObjectClassKey, ObjectCreationArgs);
	}
	catch (const CException& e)
	{
		Log::Error("ObjectsFactory: Unable to create object [%s]", objectUUID.CStr());
		return nullptr;
	}

	CreateObject(storedGuid.GetFactoryKey(), storedGuid.GetClass(), nullptr);*/

	return std::shared_ptr<IObject>();
}

std::shared_ptr<IByteBuffer> CObjectsFactory::SaveObject(std::shared_ptr<IObject> Object)
{
	std::shared_ptr<IByteBuffer> byteBuffer = std::make_shared<CByteBuffer>();
	byteBuffer->write(&cObjectBinarySignatureBegin);

	Guid guid(Object->GetType(), Object->GetClass(), 1u);
	uint64 guidRaw = guid.GetRawValue();
	byteBuffer->write(&guidRaw);

	byteBuffer->writeString(Object->GetName());

	if (auto objectLoadSave = std::dynamic_pointer_cast<IObjectLoadSave>(Object))
	{
		objectLoadSave->Save(byteBuffer);
	}

	byteBuffer->write(&cObjectBinarySignatureEnd);

	return byteBuffer;
}
