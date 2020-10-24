#include "stdafx.h"

// General
#include "ObjectClassFactory.h"

// Additional
#include "Files/ByteBuffer.h"
#include "XML/XMLManager.h"

const uint32 cObjectBinarySignatureBegin = 0x01231000;
const const char* cObjectXMLSignatureBegin = "Object";

CObjectClassFactory::CObjectClassFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: m_BaseManager(BaseManager)
	, m_TypeName(TypeName)
	, m_Type(Type)
	, m_Counter(0)
{}

CObjectClassFactory::~CObjectClassFactory()
{}



std::unordered_map<ObjectClass, std::shared_ptr<IObjectClassCreator>> CObjectClassFactory::GetClassCreators() const
{
	std::unordered_map<ObjectClass, std::shared_ptr<IObjectClassCreator>> creators;
	for (const auto& creator : m_ClassCreators)
		creators.insert(std::make_pair(creator.first, creator.second.second));

	return creators;
}

//
// IObjectClassFactory
//
std::shared_ptr<IObjectClassCreator> CObjectClassFactory::GetClassCreator(ObjectClass ObjectClassKey) const
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

std::string CObjectClassFactory::GetTypeName() const
{
	return m_TypeName;
}

ObjectType CObjectClassFactory::GetType() const
{
	return m_Type;
}

Guid CObjectClassFactory::GenerateGuid(ObjectClass ObjectClassKey)
{
	return Guid(GetType(), ObjectClassKey, ++m_Counter);
}

std::shared_ptr<IObject> CObjectClassFactory::CreateObject(ObjectClass ObjectClassKey, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto objectUUID = GenerateGuid(ObjectClassKey);

	auto it = m_ClassCreators.find(ObjectClassKey);
	if (it == m_ClassCreators.end())
		throw CException("ClassFactory: Unable find ClassCreator for '%d' in ClassFactory '%d'.", ObjectClassKey, GetType());
	
	size_t creatorIndex = it->second.first;
	auto creatorObject = it->second.second;

	auto createdObject = creatorObject->CreateObject(creatorIndex, ObjectCreationArgs);
	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdObject))
	{
		objectPrivate->SetGUID(&m_BaseManager, objectUUID);
		Log::Green("ClassFactory: Object [%s] created.", createdObject->GetName().c_str());
	}
	else
		throw CException("ClassFactory: Object [%s] not support IObjectInternal.", objectUUID.CStr());

	return createdObject;
}



//
// protected
//
Guid CObjectClassFactory::ReadGUIDXML(const std::shared_ptr<IXMLReader>& Reader)
{
	return GenerateGuid(GetBaseManager().GetManager<IObjectsFactory>()->GetObjectClassByObjectClassName(Reader->GetName()));
}

std::shared_ptr<IXMLWriter> CObjectClassFactory::WriteGUIDXML(Guid Guid)
{
	CXMLManager xmlManager(m_BaseManager);
	return xmlManager.CreateWriter(GetBaseManager().GetManager<IObjectsFactory>()->GetObjectClassNameByObjectClass(Guid.GetObjectClass()));
}

Guid CObjectClassFactory::ReadGUID(const std::shared_ptr<IByteBuffer>& Bytes)
{
	auto pos = Bytes->getPos();

	uint32 storedSignature = 0;
	Bytes->read(&storedSignature);
	if (storedSignature != cObjectBinarySignatureBegin)
		throw CException("ObjectsFactory: Bytes is not ZenonObject.");

	uint64 guidUInt = 0;
	Bytes->read(&guidUInt);
	Guid storedGuid(guidUInt);
	if (storedGuid.GetObjectType() != GetType())
		throw CException("ObjectsFactory: Stored factory mismatch!.");

	Bytes->seek(pos);
	return storedGuid;
}

std::shared_ptr<IByteBuffer> CObjectClassFactory::WriteGUID(Guid Guid)
{
	std::shared_ptr<IByteBuffer> byteBuffer = MakeShared(CByteBuffer);
	byteBuffer->write(&cObjectBinarySignatureBegin);
	byteBuffer->write(&Guid);
	return byteBuffer;
}

IBaseManager & CObjectClassFactory::GetBaseManager() const
{
	return m_BaseManager;
}
