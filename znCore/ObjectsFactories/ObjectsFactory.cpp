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
const std::unordered_map<ObjectType, std::shared_ptr<IObjectClassFactory>>& CObjectsFactory::GetClassFactories() const
{
	return m_ClassFactories;
}

std::shared_ptr<IObjectClassFactory> CObjectsFactory::GetClassFactory(ObjectType ObjectFactoryKey) const
{
	const auto& it = m_ClassFactories.find(ObjectFactoryKey);
	if (it == m_ClassFactories.end())
		throw CException("ObjectsFactory : ClassFactory '%d' not found.", ObjectFactoryKey);
	return it->second;
}

void CObjectsFactory::AddClassFactory(std::shared_ptr<IObjectClassFactory> Factory)
{
	auto creatorType = Factory->GetType();
	const auto& it = m_ClassFactories.find(creatorType);
	if (it != m_ClassFactories.end())
		throw CException("ObjectsFactory: ClassFactory '%d' already exists!", creatorType);
	m_ClassFactories.insert(std::make_pair(creatorType, Factory));
}

void CObjectsFactory::RemoveClassFactory(std::shared_ptr<IObjectClassFactory> Factory)
{
	auto creatorType = Factory->GetType();
	const auto& it = m_ClassFactories.find(creatorType);
	if (it == m_ClassFactories.end())
		throw CException("ObjectsFactory: Unable to remove ClassFactory '%d' because not found in ObjectFactory.", creatorType);
	m_ClassFactories.erase(it);
}

std::shared_ptr<IObject> CObjectsFactory::CreateObject(ObjectType ObjectTypeKey, ObjectClass ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs)
{
	Guid objectUUID(ObjectTypeKey, ObjectClassKey, 0u);

	try
	{
		return GetClassFactory(ObjectTypeKey)->CreateObject(ObjectClassKey, ObjectCreationArgs);
	}
	catch (const CException& e)
	{
		Log::Error("ObjectsFactory: Unable to create object [%s]", objectUUID.CStr());
		return nullptr;
	}
}

ObjectType CObjectsFactory::GetObjectTypeByObjectClass(ObjectClass Class)
{
	for (const auto& fact : m_ClassFactories)
		for (const auto& creator : fact.second->GetClassCreators())
			for (size_t i = 0; i < creator.second->GetSupportedClassCount(); i++)
				if (creator.second->GetSupportedClassKey(i) == Class)
					return fact.second->GetType();
	return otObject;
}

ObjectType CObjectsFactory::GetObjectTypeByObjectTypeName(std::string TypeName)
{
	for (const auto& fact : m_ClassFactories)
		if (fact.second->GetTypeName() == TypeName)
			return fact.second->GetType();

	return otObject;
}

std::string CObjectsFactory::GetObjectTypeNameByObjectType(ObjectType ObjectType)
{
	for (const auto& fact : m_ClassFactories)
		if (fact.second->GetType() == ObjectType)
			return fact.second->GetTypeName();

	return "Object";
}

std::string CObjectsFactory::GetObjectClassNameByObjectClass(ObjectClass Class)
{
	for (const auto& fact : m_ClassFactories)
		for (const auto& creator : fact.second->GetClassCreators())
			for (size_t i = 0; i < creator.second->GetSupportedClassCount(); i++)
				if (creator.second->GetSupportedClassKey(i) == Class)
					return creator.second->GetSupportedClassName(i);
	return "Object";
}

ObjectClass CObjectsFactory::GetObjectClassByObjectClassName(const std::string& ClassName)
{
	for (const auto& fact : m_ClassFactories)
		for (const auto& creator : fact.second->GetClassCreators())
			for (size_t i = 0; i < creator.second->GetSupportedClassCount(); i++)
				if (creator.second->GetSupportedClassName(i) == ClassName)
					return creator.second->GetSupportedClassKey(i);
	return 0;
}
