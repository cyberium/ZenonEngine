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
