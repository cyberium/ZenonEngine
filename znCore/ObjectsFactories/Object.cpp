#include "stdafx.h"

// General
#include "Object.h"

bool Object::operator==(const Object& rhs) const
{
	return m_Guid == rhs.m_Guid;
}
bool Object::operator!=(const Object& rhs) const
{
	return m_Guid != rhs.m_Guid;
}

std::string Object::GetTypeName() const
{
	if (m_BaseManager != nullptr)
		return m_BaseManager->GetManager<IObjectsFactory>()->GetObjectTypeNameByObjectType(m_Guid.GetObjectType());

	return "type" + std::to_string(m_Guid.GetObjectType());
}
std::string Object::GetClassNameW() const
{
	if (m_BaseManager != nullptr)
		return m_BaseManager->GetManager<IObjectsFactory>()->GetObjectClassNameByObjectClass(m_Guid.GetObjectClass());

	return "class" + std::to_string(m_Guid.GetObjectClass());
}

//
// IObject
//
Guid Object::GetGUID() const
{
	return m_Guid;
}
std::string Object::GetName() const
{
	return m_Name;
}
void Object::SetName(const std::string& Name)
{
	m_Name = Name;
}

void Object::Copy(std::shared_ptr<IObject> Destination) const
{
	auto destinationObject = std::dynamic_pointer_cast<Object>(Destination);

	if (GetGUID().GetObjectClass() != destinationObject->GetGUID().GetObjectClass() || GetGUID().GetObjectType() != destinationObject->GetGUID().GetObjectType())
		throw std::exception(("Unable to copy object with different type and class. Source: " + GetGUID().Str() + ", Destination: " + destinationObject->GetGUID().Str()).c_str());

	destinationObject->m_Name = m_Name;
}



//
// IObjectLoadSave
//
void Object::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	_ASSERT(FALSE);
}

void Object::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	_ASSERT(FALSE);
}

void Object::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	//ObjectType type = Reader->GetUInt16Attribute("Type");
	//ObjectClass class_ = Reader->GetUInt32Attribute("Class");
	//ObjectCounterType counter = Reader->GetUInt32Attribute("Counter");
	//m_Guid = Guid(type, class_, counter);

	auto name = Reader->GetStrAttribute("Name");
	if (!name.empty())
		SetName(GetClearName(name).first);
}

void Object::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	//Writer->SetUInt16Attribute(m_Guid.GetObjectType(), "Type");
	//Writer->SetUInt32Attribute(m_Guid.GetObjectClass(), "Class");
	//Writer->SetUInt32Attribute(m_Guid.GetCounter(), "Counter");

	// TODO: save only non defaults names
	auto clearName = GetClearName(m_Name).first;
	if (!clearName.empty())
		Writer->SetStrAttribute(clearName, "Name");
}



//
// IObjectPrivate
//
void Object::SetGUID(const Guid& NewGuid)
{
	//if (GetGUID().IsEmpty() == false)
	//	throw std::exception(("Object " + GetGUID().Str() + " already has Guid.").c_str());

	if (NewGuid.IsEmpty())
		throw std::exception("Cannot assign empty GUID for object.");

	if (NewGuid.GetCounter() == 0)
		throw std::exception(("Guid " + NewGuid.Str() + " not fully initialized.").c_str());

	m_Guid = Guid(NewGuid.GetRawValue());

	if (m_Name.empty())
		m_Name = GetClassNameW() + "#" + std::to_string(GetGUID().GetCounter());
}



Object::Object()
	: m_Guid(0ull)
	, m_BaseManager(nullptr)
{}
Object::Object(const IBaseManager& BaseManager)
	: m_Guid(0ull)
	, m_BaseManager(&BaseManager)
{
}
Object::Object(ObjectType Factory, ObjectClass Class)
	: m_Guid(0ull)
	, m_BaseManager(nullptr)
{}
Object::~Object()
{}