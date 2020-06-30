#include "stdafx.h"

// General
#include "ObjectClassFactory.h"

CObjectClassFactory::CObjectClassFactory(IBaseManager& BaseManager, ObjectFactoryType Type)
	: m_BaseManager(BaseManager)
	, m_Type(Type)
	, m_Counter(0)
{}

CObjectClassFactory::~CObjectClassFactory()
{}



//
// IObjectClassFactory
//
std::shared_ptr<IObjectClassCreator> CObjectClassFactory::GetClassCreator(ObjectClassType ObjectClassKey) const
{
	const auto& it = m_ClassCreators.find(ObjectClassKey);
	if (it == m_ClassCreators.end())
		throw CException("ClassFactory : ClassCreator '%d' not found.", ObjectClassKey);
	return it->second.second;
}

void CObjectClassFactory::AddClassCreator(std::shared_ptr<IObjectClassCreator> Creator)
{
	for (size_t i = 0; i < Creator->GetSupportedClassCount(); i++)
	{
		auto classKey = Creator->GetSupportedClassKey(i);
		const auto& it = m_ClassCreators.find(classKey);
		if (it != m_ClassCreators.end())
			throw CException("ClassFactory: ClassCreator '%d' already exists in ClassFactory '%d'.", classKey, GetType());
		m_ClassCreators.insert(std::make_pair(classKey, std::make_pair(i, Creator)));
	}
}

void CObjectClassFactory::RemoveClassCreator(std::shared_ptr<IObjectClassCreator> Creator)
{
	for (size_t i = 0; i < Creator->GetSupportedClassCount(); i++)
	{
		auto classKey = Creator->GetSupportedClassKey(i);
		const auto& it = m_ClassCreators.find(classKey);
		if (it == m_ClassCreators.end())
			throw CException("ClassFactory: Unable to remove ClassCreator '%d' because not found in ClassFactory '%d'.", classKey, GetType());
		m_ClassCreators.erase(it);
	}
}

ObjectFactoryType CObjectClassFactory::GetType() const
{
	return m_Type;
}

Object::Guid CObjectClassFactory::GenerateGuid(ObjectClassType ObjectClassKey)
{
	return Object::Guid(GetType(), ObjectClassKey, ++m_Counter);
}

std::shared_ptr<IObject> CObjectClassFactory::CreateObject(ObjectClassType ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto objectUUID = GenerateGuid(ObjectClassKey);

	auto it = m_ClassCreators.find(ObjectClassKey);
	if (it == m_ClassCreators.end())
		throw CException("ClassFactory: Unable find ClassCreator for '%d' in ClassFactory '%d'.", ObjectClassKey, GetType());
	
	size_t creatorIndex = it->second.first;
	auto creatorObject = it->second.second;

	auto createdObject = creatorObject->CreateObject(creatorIndex, ObjectCreationArgs);
	if (auto objectInternal = std::dynamic_pointer_cast<IObjectInternal>(createdObject))
	{
		objectInternal->SetGuid(objectUUID.GetRawValue());
		Log::Green("ClassFactory: Object [%s] created.", createdObject->GetName().c_str());
	}
	else
		throw CException("ClassFactory: Object [%s] not support IObjectInternal.", objectUUID.CStr());

	return createdObject;
}

std::shared_ptr<IObject> CObjectClassFactory::LoadObject(ObjectClassType ObjectClassKey, std::shared_ptr<IByteBuffer> Bytes)
{
	auto objectUUID = GenerateGuid(ObjectClassKey);

	auto it = m_ClassCreators.find(ObjectClassKey);
	if (it == m_ClassCreators.end())
		throw CException("ClassFactory: Unable find ClassCreator for '%d' in ClassFactory '%d'.", ObjectClassKey, GetType());

	size_t creatorIndex = it->second.first;
	auto creatorObject = it->second.second;

	auto loadedObject = creatorObject->CreateObject(creatorIndex, nullptr);
	if (auto objectInternal = std::dynamic_pointer_cast<IObjectInternal>(loadedObject))
	{
		objectInternal->SetGuid(objectUUID.GetRawValue());
		Log::Green("ClassFactory: Object [%s] loaded.", loadedObject->GetName().c_str());
	}
	else
		throw CException("ClassFactory: Object [%s] not support IObjectInternal.", objectUUID.CStr());

	return loadedObject;
}

std::shared_ptr<IByteBuffer> CObjectClassFactory::SaveObject(std::shared_ptr<IObject> Object)
{
	return std::shared_ptr<IByteBuffer>();
}
