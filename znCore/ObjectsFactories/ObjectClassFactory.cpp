#include "stdafx.h"

// General
#include "ObjectClassFactory.h"

// Additional
#include "Files/ByteBuffer.h"
#include "XML/XMLManager.h"

const uint32 cObjectBinarySignatureBegin = 0x01231000;
const char* cObjectXMLSignatureBegin = "Object";

CObjectClassFactory::CObjectClassFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: m_BaseManager(BaseManager)
	, m_TypeName(TypeName)
	, m_Type(Type)
	, m_Counter(0)
{}

CObjectClassFactory::~CObjectClassFactory()
{}



//
// IObjectClassFactory
//
std::vector<std::shared_ptr<IObjectClassCreator>> CObjectClassFactory::GetClassCreators() const
{
	return m_ClassCreators;
}

std::shared_ptr<IObjectClassCreator> CObjectClassFactory::GetClassCreator(ObjectClass ObjectClassKey) const
{
	for (const auto& crIt : m_ClassCreators)
		if (crIt->IsCanCreate(ObjectClassKey))
			return crIt;

	throw CException("ClassFactory : ClassCreator '%d' not found.", ObjectClassKey);
}

void CObjectClassFactory::AddClassCreator(std::shared_ptr<IObjectClassCreator> Creator)
{
	const auto& crIt = std::find(m_ClassCreators.begin(), m_ClassCreators.end(), Creator);
	if (crIt != m_ClassCreators.end())
		throw CException("ClassFactory: ClassCreator already exists in ClassFactory '%d'.", GetType());
	m_ClassCreators.push_back(Creator);
}

void CObjectClassFactory::RemoveClassCreator(std::shared_ptr<IObjectClassCreator> Creator)
{
	const auto& crIt = std::find(m_ClassCreators.begin(), m_ClassCreators.end(), Creator);
	if (crIt == m_ClassCreators.end())
		throw CException("ClassFactory: Unable to remove ClassCreator because not found in ClassFactory '%d'.", GetType());
	m_ClassCreators.erase(crIt);
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
	for (const auto& crIt : m_ClassCreators)
		if (crIt->IsCanCreate(ObjectClassKey))
			return crIt->CreateObject(ObjectClassKey, GenerateGuid(ObjectClassKey), ObjectCreationArgs);

	throw CException("ClassFactory: Unable find ClassCreator for '%d' in ClassFactory '%d'.", ObjectClassKey, GetType());
}



//
// protected
//
Guid CObjectClassFactory::ReadGUIDXML(const std::shared_ptr<IXMLReader>& Reader)
{
	std::string objectClassName = Reader->GetName();
	_ASSERT(false == objectClassName.empty());

	ObjectClass objectClass = GetBaseManager().GetManager<IObjectsFactory>()->GetObjectClassByObjectClassName(objectClassName);
	return GenerateGuid(objectClass);
}

std::shared_ptr<IXMLWriter> CObjectClassFactory::WriteGUIDXML(Guid Guid)
{
	CXMLManager xmlManager(GetBaseManager());
	std::string objectClassName = GetBaseManager().GetManager<IObjectsFactory>()->GetObjectClassNameByObjectClass(Guid.GetObjectClass());
	_ASSERT(false == objectClassName.empty());
	return xmlManager.CreateWriter(objectClassName);
}

Guid CObjectClassFactory::ReadGUID(const std::shared_ptr<IByteBuffer>& Bytes)
{
	auto pos = Bytes->getPos();

	try
	{
		uint32 storedSignature = 0;
		if (false == Bytes->read(&storedSignature))
			throw CException("Incorrect bytes size.");

		if (storedSignature != cObjectBinarySignatureBegin)
			throw CException("Bytes is not ZenonObject.");

		uint64 guidUInt = 0;
		if (false == Bytes->read(&guidUInt))
			throw CException("Incorrect bytes size.");

		Guid storedGuid(guidUInt);
		if (storedGuid.GetObjectType() != GetType())
			throw CException("Stored factory mismatch!.");

		return storedGuid;
	}
	catch (const CException& e)
	{
		Bytes->seek(pos);
		throw;
	}
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
